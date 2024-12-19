/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated January 1, 2020. Replaces all prior versions.
 *
 * Copyright (c) 2013-2020, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THE SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifdef SPINE_UE4
    #include "SpinePluginPrivatePCH.h"
#endif

#include <spine/TwoColorTimeline.h>

#include <spine/Event.h>
#include <spine/Skeleton.h>

#include <spine/Animation.h>
#include <spine/Bone.h>
#include <spine/Slot.h>
#include <spine/SlotData.h>

using namespace spine;

RTTI_IMPL(TwoColorTimeline, CurveTimeline)

const int TwoColorTimeline::ENTRIES = 8;
const int TwoColorTimeline::PREV_TIME = -8;
const int TwoColorTimeline::PREV_R = -7;
const int TwoColorTimeline::PREV_G = -6;
const int TwoColorTimeline::PREV_B = -5;
const int TwoColorTimeline::PREV_A = -4;
const int TwoColorTimeline::PREV_R2 = -3;
const int TwoColorTimeline::PREV_G2 = -2;
const int TwoColorTimeline::PREV_B2 = -1;
const int TwoColorTimeline::R = 1;
const int TwoColorTimeline::G = 2;
const int TwoColorTimeline::B = 3;
const int TwoColorTimeline::A = 4;
const int TwoColorTimeline::R2 = 5;
const int TwoColorTimeline::G2 = 6;
const int TwoColorTimeline::B2 = 7;

TwoColorTimeline::TwoColorTimeline(int frameCount,size_t bezierCount) : CurveTimeline(frameCount,ENTRIES,bezierCount), _slotIndex(0) {
    _frames.ensureCapacity(frameCount * ENTRIES);
    _frames.setSize(frameCount * ENTRIES, 0);
}

float TwoColorTimeline::getCurveComponentValue(float time, size_t componentOffset) {
    int i = (int)_frames.size() - ENTRIES;
    for (int ii = ENTRIES; ii <= i; ii += ENTRIES) {
        if (_frames[ii] > time) {
            i = ii - ENTRIES;
            break;
        }
    }

    int curveType = (int)_curves[i / ENTRIES];
    switch (curveType) {
        case CurveTimeline::LINEAR: {
            float before = _frames[i], value = _frames[i + componentOffset];
            return value + (time - before) / (_frames[i + ENTRIES] - before) *
                           (_frames[i + ENTRIES + componentOffset] - value);
        }
        case CurveTimeline::STEPPED:
            return _frames[i + componentOffset];
    }

    // Handle Bezier curve interpolation for the component
    return getBezierValue(time, i, componentOffset, curveType - CurveTimeline::BEZIER);
}

void TwoColorTimeline::apply(Skeleton &skeleton, float lastTime, float time, Vector<Event *> *pEvents, float alpha,
                             MixBlend blend, MixDirection direction) {
    SP_UNUSED(lastTime);
    SP_UNUSED(pEvents);
    SP_UNUSED(direction);

    Slot *slotP = skeleton._slots[_slotIndex];
    Slot &slot = *slotP;
    if (!slot._bone.isActive()) return;

    Color &color = slot.getColor();
    Color &darkColor = slot.getDarkColor();
    const Color &dataColor = slot._data.getColor();
    const Color &dataDarkColor = slot._data.getDarkColor();

    if (time < _frames[0]) {
        // Handle time before the first frame
        switch (blend) {
            case MixBlend_Setup:
                color.set(dataColor);
                darkColor.set(dataDarkColor);
                return;
            case MixBlend_First:
                color.r += (dataColor.r - color.r) * alpha;
                color.g += (dataColor.g - color.g) * alpha;
                color.b += (dataColor.b - color.b) * alpha;
                color.a += (dataColor.a - color.a) * alpha;

                darkColor.r += (dataDarkColor.r - darkColor.r) * alpha;
                darkColor.g += (dataDarkColor.g - darkColor.g) * alpha;
                darkColor.b += (dataDarkColor.b - darkColor.b) * alpha;

                return;
            default:
                return;
        }
    }

    // Interpolated values for each color component
    float r = getCurveComponentValue(time, PREV_R);
    float g = getCurveComponentValue(time, PREV_G);
    float b = getCurveComponentValue(time, PREV_B);
    float a = getCurveComponentValue(time, PREV_A);
    float r2 = getCurveComponentValue(time, PREV_R2);
    float g2 = getCurveComponentValue(time, PREV_G2);
    float b2 = getCurveComponentValue(time, PREV_B2);

    if (alpha == 1) {
        color.set(r, g, b, a);
        darkColor.set(r2, g2, b2, 1);
    } else {
        Color &light = color;
        Color &dark = darkColor;
        if (blend == MixBlend_Setup) {
            color.set(dataColor);
            darkColor.set(dataDarkColor);
        }
        light.add((r - light.r) * alpha, (g - light.g) * alpha, (b - light.b) * alpha, (a - light.a) * alpha);
        dark.add((r2 - dark.r) * alpha, (g2 - dark.g) * alpha, (b2 - dark.b) * alpha, 0);
    }
}

void TwoColorTimeline::setFrame(int frameIndex, float time, float r, float g, float b, float a, float r2, float g2, float b2) {
    frameIndex *= ENTRIES;
    _frames[frameIndex] = time;
    _frames[frameIndex + R] = r;
    _frames[frameIndex + G] = g;
    _frames[frameIndex + B] = b;
    _frames[frameIndex + A] = a;
    _frames[frameIndex + R2] = r2;
    _frames[frameIndex + G2] = g2;
    _frames[frameIndex + B2] = b2;
}

int TwoColorTimeline::getSlotIndex() {
    return _slotIndex;
}

void TwoColorTimeline::setSlotIndex(int inValue) {
    assert(inValue >= 0);
    _slotIndex = inValue;
}