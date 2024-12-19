#include <emscripten/bind.h>
#include <emscripten/wire.h>
#include <cstdint>
#include <type_traits>
#include "spine-skeleton-instance.h"
#include "spine-wasm.h"
#include "Vector2.h"

using namespace spine;

using SPVectorFloat = Vector<float>;
using SPVectorVectorFloat = Vector<Vector<float>>;
using SPVectorInt = Vector<int>;
using SPVectorUint = Vector<unsigned int>;
using SPVectorVectorInt = Vector<Vector<int>>;
using SPVectorSize_t = Vector<size_t>;
using SPVectorBonePtr = Vector<Bone*>;
using SPVectorBoneDataPtr = Vector<BoneData*>;
using SPVectorSlotDataPtr = Vector<SlotData*>;
using SPVectorTransformConstraintDataPtr = Vector<TransformConstraintData*>;
using SPVectorPathConstraintDataPtr = Vector<PathConstraintData*>;
using SPVectorUnsignedShort = Vector<unsigned short>;
using SPVectorSPString = Vector<String>;
using SPVectorConstraintDataPtr = Vector<ConstraintData*>;
using SPVectorSlotPtr = Vector<Slot*>;
using SPVectorSkinPtr = Vector<Skin*>;
using SPVectorEventDataPtr = Vector<EventData*>;
using SPVectorEventPtr = Vector<spine::Event*>;
using SPVectorAnimationPtr = Vector<Animation*>;
using SPVectorIkConstraintPtr = Vector<IkConstraint*>;
using SPVectorIkConstraintDataPtr = Vector<IkConstraintData*>;
using SPVectorTransformConstraintPtr = Vector<TransformConstraint*>;
using SPVectorPathConstraintPtr = Vector<PathConstraint*>;
using SPVectorTimelinePtr = Vector<Timeline*>;
using SPVectorTrackEntryPtr = Vector<TrackEntry*>;
using SPVectorUpdatablePtr = Vector<Updatable*>;
using SPVectorSkinEntryPtr = Vector<Skin::AttachmentMap::Entry*>;
using SPVectorVectorSkinEntryPtr = Vector<SPVectorSkinEntryPtr>;
using SPVectorDebugShape = Vector<SpineDebugShape>;

#define DEFINE_ALLOW_RAW_POINTER(type) \
namespace emscripten { namespace internal { \
    template<> \
    struct TypeID<type*> { \
        static constexpr TYPEID get() { \
            return TypeID<type>::get(); \
        } \
    }; \
    template<> \
    struct TypeID<const type*> { \
        static constexpr TYPEID get() { \
            return TypeID<type>::get(); \
        } \
    }; \
}}


#define DEFINE_SPINE_CLASS_TYPEID(cls) \
namespace emscripten { namespace internal { \
    template<> \
    constexpr TYPEID getLightTypeID<cls>(const cls& value) { \
        return value.getRTTI().getClassName(); \
    } \
    template<> \
    struct LightTypeID<cls* const> { \
        static constexpr TYPEID get() { \
            return #cls "*"; \
        } \
    }; \
    template<> \
    struct LightTypeID<cls*> { \
        static constexpr TYPEID get() { \
            return #cls "*"; \
        } \
    }; \
    template<> \
    struct LightTypeID<cls* const &> { \
        static constexpr TYPEID get() { \
            return #cls "*"; \
        } \
    }; \
    template<> \
    struct LightTypeID<cls*&> { \
        static constexpr TYPEID get() { \
            return #cls "*"; \
        } \
    }; \
    template<> \
    struct LightTypeID<const cls*> { \
        static constexpr TYPEID get() { \
            return "const " #cls "*"; \
        } \
    }; \
    template<> \
    struct LightTypeID<const cls* const> { \
        static constexpr TYPEID get() { \
            return "const " #cls "*"; \
        } \
    }; \
    template<> \
    struct LightTypeID<const cls* &> { \
        static constexpr TYPEID get() { \
            return "const " #cls "*"; \
        } \
    }; \
    template<> \
    struct LightTypeID<const cls* const &> { \
        static constexpr TYPEID get() { \
            return "const " #cls "*"; \
        } \
    }; \
    template<> \
    struct LightTypeID<cls> { \
        static constexpr TYPEID get() { \
            return #cls; \
        } \
    }; \
    template<> \
    struct LightTypeID<cls&> { \
        static constexpr TYPEID get() { \
            return #cls; \
        } \
    }; \
    template<> \
    struct LightTypeID<const cls> { \
        static constexpr TYPEID get() { \
            return #cls; \
        } \
    }; \
    template<> \
    struct LightTypeID<const cls&> { \
        static constexpr TYPEID get() { \
            return #cls; \
        } \
    }; \
}}

#define GETTER_RVAL_TO_PTR(ClassType, Method, ReturnType) \
    optional_override([](ClassType &obj) { return const_cast<ReturnType>(&obj.Method()); })

DEFINE_SPINE_CLASS_TYPEID(Color)
DEFINE_SPINE_CLASS_TYPEID(ConstraintData)
DEFINE_SPINE_CLASS_TYPEID(EventData)
DEFINE_SPINE_CLASS_TYPEID(IkConstraintData)
DEFINE_SPINE_CLASS_TYPEID(PathConstraintData)
DEFINE_SPINE_CLASS_TYPEID(Attachment)
DEFINE_SPINE_CLASS_TYPEID(VertexAttachment)
DEFINE_SPINE_CLASS_TYPEID(BoundingBoxAttachment)
DEFINE_SPINE_CLASS_TYPEID(ClippingAttachment)
DEFINE_SPINE_CLASS_TYPEID(MeshAttachment)
DEFINE_SPINE_CLASS_TYPEID(PathAttachment)
DEFINE_SPINE_CLASS_TYPEID(PointAttachment)
DEFINE_SPINE_CLASS_TYPEID(RegionAttachment)
DEFINE_SPINE_CLASS_TYPEID(AttachmentLoader)
DEFINE_SPINE_CLASS_TYPEID(AtlasAttachmentLoader)
DEFINE_SPINE_CLASS_TYPEID(Updatable)
DEFINE_SPINE_CLASS_TYPEID(IkConstraint)
DEFINE_SPINE_CLASS_TYPEID(PathConstraint)
DEFINE_SPINE_CLASS_TYPEID(TransformConstraintData)
DEFINE_SPINE_CLASS_TYPEID(TransformConstraint)
DEFINE_SPINE_CLASS_TYPEID(Bone)
DEFINE_SPINE_CLASS_TYPEID(Timeline)
DEFINE_SPINE_CLASS_TYPEID(CurveTimeline)
DEFINE_SPINE_CLASS_TYPEID(TranslateTimeline)
DEFINE_SPINE_CLASS_TYPEID(ScaleTimeline)
DEFINE_SPINE_CLASS_TYPEID(ShearTimeline)
DEFINE_SPINE_CLASS_TYPEID(RotateTimeline)
DEFINE_SPINE_CLASS_TYPEID(RGBATimeline)
DEFINE_SPINE_CLASS_TYPEID(TwoColorTimeline)
DEFINE_SPINE_CLASS_TYPEID(AttachmentTimeline)
DEFINE_SPINE_CLASS_TYPEID(DeformTimeline)
DEFINE_SPINE_CLASS_TYPEID(EventTimeline)
DEFINE_SPINE_CLASS_TYPEID(DrawOrderTimeline)
DEFINE_SPINE_CLASS_TYPEID(IkConstraintTimeline)
DEFINE_SPINE_CLASS_TYPEID(TransformConstraintTimeline)
DEFINE_SPINE_CLASS_TYPEID(PathConstraintPositionTimeline)
DEFINE_SPINE_CLASS_TYPEID(PathConstraintMixTimeline)
//DEFINE_SPINE_CLASS_TYPEID(VertexEffect)
//DEFINE_SPINE_CLASS_TYPEID(JitterVertexEffect)
//DEFINE_SPINE_CLASS_TYPEID(SwirlVertexEffect)


DEFINE_ALLOW_RAW_POINTER(BoneData)
DEFINE_ALLOW_RAW_POINTER(Bone)
DEFINE_ALLOW_RAW_POINTER(Slot)
DEFINE_ALLOW_RAW_POINTER(SlotData)
DEFINE_ALLOW_RAW_POINTER(VertexAttachment)
DEFINE_ALLOW_RAW_POINTER(Attachment)
DEFINE_ALLOW_RAW_POINTER(Color)
DEFINE_ALLOW_RAW_POINTER(EventData)
DEFINE_ALLOW_RAW_POINTER(Skeleton)
DEFINE_ALLOW_RAW_POINTER(SkeletonData)
DEFINE_ALLOW_RAW_POINTER(Skin)
DEFINE_ALLOW_RAW_POINTER(Animation)
DEFINE_ALLOW_RAW_POINTER(AnimationStateData)
DEFINE_ALLOW_RAW_POINTER(TrackEntry)
DEFINE_ALLOW_RAW_POINTER(IkConstraintData)
DEFINE_ALLOW_RAW_POINTER(PathConstraintData)
DEFINE_ALLOW_RAW_POINTER(TransformConstraintData)
DEFINE_ALLOW_RAW_POINTER(SPVectorUnsignedShort)
DEFINE_ALLOW_RAW_POINTER(SPVectorFloat)

namespace {

template <typename T>
void registerSpineInteger(const char* name) {
    using namespace emscripten::internal;
    using UnderlyingType = typename std::underlying_type<T>::type;
    _embind_register_integer(TypeID<T>::get(), name, sizeof(T), std::numeric_limits<UnderlyingType>::min(),
    std::numeric_limits<UnderlyingType>::max());
}

#define REGISTER_SPINE_ENUM(name) \
    registerSpineInteger<spine::name>("spine::" #name)


template<typename T, bool>
struct SpineVectorTrait {};

template<typename T>
struct SpineVectorTrait<T, false> {
    static emscripten::class_<spine::Vector<T>> register_spine_vector(const char* name) {
        typedef spine::Vector<T> VecType;

        void (VecType::*setSize)(const size_t, const T&) = &VecType::setSize;
        size_t (VecType::*size)() const = &VecType::size;
        T& (VecType::*get)(size_t) = &VecType::operator[];
        return emscripten::class_<spine::Vector<T>>(name)
            .template constructor<>()
            .function("resize", setSize)
            .function("size", size)
            .function("get", get, emscripten::allow_raw_pointers());
    }
};

template<typename T>
struct SpineVectorTrait<T, true> {
    static emscripten::class_<spine::Vector<T>> register_spine_vector(const char* name) {
        typedef spine::Vector<T> VecType;

        void (VecType::*setSize)(const size_t, const T&) = &VecType::setSize;
        size_t (VecType::*size)() const = &VecType::size;
        T& (VecType::*get)(size_t) = &VecType::operator[];
        return emscripten::class_<spine::Vector<T>>(name)
            .template constructor<>()
            .function("resize", setSize)
            .function("size", size)
            .function("get", get)
            .function("set", emscripten::optional_override([](VecType& obj, int index, const T& value){
                obj[index] = value;
            }), emscripten::allow_raw_pointers());
    }
};

#define REGISTER_SPINE_VECTOR(name, needSetter) \
    SpineVectorTrait<name::value_type, needSetter>::register_spine_vector(#name)


} // namespace

namespace emscripten { namespace internal {

template<typename GetterReturnType, typename GetterThisType>
struct GetterPolicy<GetterReturnType (GetterThisType::*)()> {
    using ReturnType = GetterReturnType;
    using Context = GetterReturnType (GetterThisType::*)();

    using Binding = internal::BindingType<ReturnType>;
    using WireType = typename Binding::WireType;

    template<typename ClassType, typename ReturnPolicy>
    //template<typename ClassType>
    static WireType get(const Context& context, ClassType& ptr) {
        return Binding::toWireType(((ptr.*context)()), ReturnPolicy{});
        //return Binding::toWireType(((ptr.*context)()));
    }

    static void* getContext(Context context) {
        return internal::getContext(context);
    }
};

// Non-const version
template<typename GetterReturnType, typename GetterThisType>
struct GetterPolicy<GetterReturnType (*)(GetterThisType&)> {
    using ReturnType = GetterReturnType;
    using Context = GetterReturnType (*)(GetterThisType &);

    using Binding = internal::BindingType<ReturnType>;
    using WireType = typename Binding::WireType;

    template<typename ClassType>
    static WireType get(const Context& context, ClassType& ptr) {
        return Binding::toWireType(context(ptr));
    }

    static void* getContext(Context context) {
        return internal::getContext(context);
    }
};

template<>
struct BindingType<String> {
    using T = char;
    static_assert(std::is_trivially_copyable<T>::value, "basic_string elements are memcpy'd");
    using WireType = struct {
        size_t length;
        T data[1]; // trailing data
    } *;
    static WireType toWireType(const String& v,emscripten::internal::rvp::default_tag) {
        auto* wt = static_cast<WireType>(malloc(sizeof(size_t) + v.length() * sizeof(T)));
        wt->length = v.length();
        memcpy(wt->data, v.buffer(), v.length() * sizeof(T));
        return wt;
    }
    static String fromWireType(WireType v) {
        return String(v->data, v->length, false);
    }
};

}  // namespace internal
}  // namespace emscripten

#define ENABLE_EMBIND_TEST 0

#if ENABLE_EMBIND_TEST

class TestBase {
    RTTI_DECL
public:
    virtual void hello(const String& msg) {
        printf("TestBase::hello: %s\n", msg.buffer());
    }
};

RTTI_IMPL_NOPARENT(TestBase)

class TestFoo: public TestBase {
    RTTI_DECL
public:
    TestFoo() {
        printf("TestFoo::TestFoo: %p\n", this);
    }

    TestFoo(const TestFoo& o) {
        printf("TestFoo copy constructor %p, %p, o.x=%d\n", this, &o, o._x);
        *this = o;
    }

    ~TestFoo() {
        printf("TestFoo::~TestFoo: %p\n", this);
    }

    TestFoo &operator=(const TestFoo& o) {
        printf("TestFoo::operator=: %p\n", this);
        if (this != &o) {
            _x = o._x;
            printf("TestFoo::operator=, _x=%d\n", _x);
        } else {
            printf("TestFoo::operator=, same address\n");
        }
        return *this;
    }

    virtual void hello(const String& msg) override {
        printf("TestFoo::hello: %s\n", msg.buffer());
    }

    void setX(int x) { 
        _x = x;
    }
    int getX() const { 
        return _x;
    }

    static void apply(int a, int b) {
        printf("apply1, a: %d, b: %d\n", a, b);
    }
    static void apply(const Vector<int>& a, bool b) {
        printf("apply2, a, size: %d, b: %d\n", (int)a.size(), b);
        for (int i = 0; i < a.size(); ++i) {
            printf("apply2, aaa: [%d]=%d\n", i, a[i]);
        }
    }
private:
    int _x = 0;
};

RTTI_IMPL(TestFoo, TestBase)

DEFINE_SPINE_CLASS_TYPEID(TestBase)
DEFINE_SPINE_CLASS_TYPEID(TestFoo)

DEFINE_ALLOW_RAW_POINTER(TestBase)
DEFINE_ALLOW_RAW_POINTER(TestFoo)

class TestBar {
public:
    RTTI_DECL
    TestBar() {
        printf("TestBar::TestBar: %p\n", this);
    }

    TestBar(const TestBar& o) {
        printf("TestBar copy constructor %p\n", this);
        *this = o;
    }

    ~TestBar() {
        printf("TestBar::~TestBar: %p\n", this);
        delete _foo;
    }

    TestBar &operator=(const TestBar& o) {
        printf("TestBar::operator=: %p\n", this);
        if (this != &o) {
            _foo = o._foo;
        }
        return *this;
    }

    const TestFoo* getFoo() const {
        return _foo;
    }

    void setFoo(TestFoo *foo) {
        if (_foo != foo) {
            delete _foo;
            _foo = foo;
        }
    }

   const TestBase* getBase() const {
        return _foo;
    }

    const TestFoo& getFooConst() {
        return *_foo;
    }

    void setFooConst(const TestFoo& foo) {
        _foo = &foo;
    }

    Vector<String> getNames() {
        Vector<String> ret;
        ret.add(String("你好"));
        ret.add(String("World"));
        return ret;
    }

private:
    const TestFoo *_foo = new TestFoo();
};

RTTI_IMPL_NOPARENT(TestBar)


#endif // ENABLE_EMBIND_TEST


EMSCRIPTEN_BINDINGS(spine) {
    using namespace emscripten;
    using namespace emscripten::internal;

#if ENABLE_EMBIND_TEST
    class_<TestBase>("TestBase")
        .constructor()
        .function("hello", &TestBase::hello);

    class_<TestFoo, base<TestBase>>("TestFoo")
        .constructor()
        .property("x", &TestFoo::getX, &TestFoo::setX)
        .class_function("apply1", select_overload<void (int, int)>(&TestFoo::apply))
        .class_function("apply2", select_overload<void (const Vector<int> &, bool)>(&TestFoo::apply))
        ;

    class_<TestBar>("TestBar")
        .constructor()
        .property("foo", &TestBar::getFoo, &TestBar::setFoo)
        .property("base", &TestBar::getBase)
        .function("getBase", &TestBar::getBase, allow_raw_pointers())
        .function("getFooConst", &TestBar::getFooConst, allow_raw_pointers())
        .function("setFooConst", &TestBar::setFooConst, allow_raw_pointers())
        .function("getNames", &TestBar::getNames)
        .property("names", &TestBar::getNames)
        ;

    Json json(R"({"smile": "\uD83D\uDE0A🇨🇳  \uD83D\uDE00 \uD83D\uDE02  \uD83D\uDE21 "})");
    const char *smileValue = Json::getString(&json, R"(smile)", "");
    printf(">>> smile value: %s\n", smileValue);

#endif // ENABLE_EMBIND_TEST

	_embind_register_std_string(TypeID<spine::String>::get(), "std::string");

    //REGISTER_SPINE_ENUM(TimelineType);
    REGISTER_SPINE_ENUM(MixDirection);
    REGISTER_SPINE_ENUM(MixBlend);
    REGISTER_SPINE_ENUM(EventType);
    REGISTER_SPINE_ENUM(BlendMode);
    REGISTER_SPINE_ENUM(Inherit);
    REGISTER_SPINE_ENUM(PositionMode);
    REGISTER_SPINE_ENUM(SpacingMode);
    REGISTER_SPINE_ENUM(RotateMode);
    REGISTER_SPINE_ENUM(TextureFilter);
    REGISTER_SPINE_ENUM(TextureWrap);
    REGISTER_SPINE_ENUM(AttachmentType);


    REGISTER_SPINE_VECTOR(SPVectorDebugShape, false);

    REGISTER_SPINE_VECTOR(SPVectorFloat, true);
    REGISTER_SPINE_VECTOR(SPVectorVectorFloat, true);
    REGISTER_SPINE_VECTOR(SPVectorInt, true);
    REGISTER_SPINE_VECTOR(SPVectorUint, true);
    REGISTER_SPINE_VECTOR(SPVectorVectorInt, true);
    REGISTER_SPINE_VECTOR(SPVectorSize_t, true);
    REGISTER_SPINE_VECTOR(SPVectorUnsignedShort, true);

    REGISTER_SPINE_VECTOR(SPVectorSPString, true);
    REGISTER_SPINE_VECTOR(SPVectorBonePtr, false);
    REGISTER_SPINE_VECTOR(SPVectorBoneDataPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorSlotDataPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorTransformConstraintDataPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorPathConstraintDataPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorConstraintDataPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorSlotPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorSkinPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorEventDataPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorEventPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorAnimationPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorIkConstraintPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorIkConstraintDataPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorTransformConstraintPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorPathConstraintPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorTimelinePtr, true); // .set used in Animation constructor 
    REGISTER_SPINE_VECTOR(SPVectorTrackEntryPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorUpdatablePtr, false);
    REGISTER_SPINE_VECTOR(SPVectorSkinEntryPtr, false);
    REGISTER_SPINE_VECTOR(SPVectorVectorSkinEntryPtr, false);

    class_<Vector2>("Vector2")
        .constructor<>()
        .constructor<float, float>()
        .property("x", &Vector2::x)
        .property("y", &Vector2::y)
        .function("set", &Vector2::set)
        .function("length", &Vector2::length)
        .function("normalize", &Vector2::normalize);

    class_<Color>("Color")
        .constructor<>()
        .constructor<float, float, float, float>()
        .function("set", select_overload<Color& (float, float, float, float)>(&Color::set))
        .function("add", select_overload<Color& (float, float, float, float)>(&Color::add))
        .function("clamp", &Color::clamp)
        .property("r", &Color::r)
        .property("g", &Color::g)
        .property("b", &Color::b)
        .property("a", &Color::a);

    class_<Interpolation>("Interpolation")
        .function("apply", &Interpolation::apply, pure_virtual());

    class_<HasRendererObject>("HasRendererObject")
        .constructor<>();

    // class_<Triangulator>("Triangulator")
    //     .constructor<>()
    //     .function("triangulate", &Triangulator::triangulate)
    //     .function("decompose", &Triangulator::decompose, allow_raw_pointers());

    class_<ConstraintData>("ConstraintData")
        .constructor<const String &>()
        .property("name", &ConstraintData::getName)
        .property("order", &ConstraintData::getOrder, &ConstraintData::setOrder)
        .property("skinRequired", &ConstraintData::isSkinRequired, &ConstraintData::setSkinRequired);

    class_<IkConstraintData, base<ConstraintData>>("IkConstraintData")
        .constructor<const String &>()
        .function("getBones", optional_override([](IkConstraintData &obj) {
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBoneDataPtr>()) 
        .property("target", &IkConstraintData::getTarget, &IkConstraintData::setTarget)
        .property("bendDirection", &IkConstraintData::getBendDirection, &IkConstraintData::setBendDirection)
        .property("compress", &IkConstraintData::getCompress, &IkConstraintData::setCompress)
        .property("stretch", &IkConstraintData::getStretch, &IkConstraintData::setStretch)
        .property("uniform", &IkConstraintData::getUniform, &IkConstraintData::setUniform)
        .property("mix", &IkConstraintData::getMix, &IkConstraintData::setMix)
        .property("softness", &IkConstraintData::getSoftness, &IkConstraintData::setSoftness);

    class_<PathConstraintData, base<ConstraintData>>("PathConstraintData")
        .constructor<const String &>()
        .function("getBones",optional_override([](PathConstraintData &obj) {
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBoneDataPtr>())
        .property("target", &PathConstraintData::getTarget, &PathConstraintData::setTarget)
        .property("positionMode", &PathConstraintData::getPositionMode, &PathConstraintData::setPositionMode)
        .property("spacingMode", &PathConstraintData::getSpacingMode, &PathConstraintData::setSpacingMode)
        .property("rotateMode", &PathConstraintData::getRotateMode, &PathConstraintData::setRotateMode)
        .property("offsetRotation", &PathConstraintData::getOffsetRotation, &PathConstraintData::setOffsetRotation)
        .property("position", &PathConstraintData::getPosition, &PathConstraintData::setPosition)
        .property("spacing", &PathConstraintData::getSpacing, &PathConstraintData::setSpacing)
        .property("mixRotate", &PathConstraintData::getMixRotate, &PathConstraintData::setMixRotate)
        .property("mixX", &PathConstraintData::getMixX, &PathConstraintData::setMixX)
		.property("mixY", &PathConstraintData::getMixY, &PathConstraintData::setMixY);
		

    class_<SkeletonBounds>("SkeletonBounds")
        .constructor<>()
        .function("update", &SkeletonBounds::update)
        .function("aabbContainsPoint", &SkeletonBounds::aabbcontainsPoint)
        .function("aabbIntersectsSegment", &SkeletonBounds::aabbintersectsSegment)
        .function("aabbIntersectsSkeleton", &SkeletonBounds::aabbIntersectsSkeleton)
        .function("containsPoint", optional_override([](SkeletonBounds &obj, float x, float y) {
            return obj.containsPoint(x, y); }),allow_raw_pointers())
        // .function("containsPointPolygon", optional_override([](SkeletonBounds &obj,Polygon* polygon, float x, float y) {
            // return obj.containsPoint(polygon, x, y); }),allow_raw_pointers())
        .function("intersectsSegment", optional_override([](SkeletonBounds &obj, float x1, float y1, float x2, float y2){
            return obj.intersectsSegment(x1, y1, x2, y2); }),allow_raw_pointers())
        // .function("intersectsSegmentPolygon", optional_override([](SkeletonBounds &obj,Polygon* polygon,
        // float x1, float y1, float x2, float y2){
            // return obj.intersectsSegment(polygon, x1, y1, x2, y2); }),allow_raw_pointers())
        // .function("getPolygon", &SkeletonBounds::getPolygon, allow_raw_pointers())
        .function("getWidth", &SkeletonBounds::getWidth)
        .function("getHeight", &SkeletonBounds::getHeight);

    class_<Event>("Event")
        .constructor<float, const EventData &>()
        .property("data", &Event::getData)
        .property("intValue", &Event::getIntValue, &Event::setIntValue)
        .property("floatValue", &Event::getFloatValue, &Event::setFloatValue)
        .property("stringValue", &Event::getStringValue, &Event::setStringValue)
        .property("time", &Event::getTime)
        .property("volume", &Event::getVolume, &Event::setVolume)
        .property("balance", &Event::getBalance, &Event::setBalance);

    class_<EventData>("EventData")
        .constructor<const String &>()
        .property("name", &EventData::getName)
        .property("intValue", &EventData::getIntValue, &EventData::setIntValue)
        .property("floatValue", &EventData::getFloatValue, &EventData::setFloatValue)
        .property("stringValue", &EventData::getStringValue, &EventData::setStringValue)
        .property("audioPath", &EventData::getAudioPath, &EventData::setAudioPath)
        .property("volume", &EventData::getVolume, &EventData::setVolume)
        .property("balance", &EventData::getBalance, &EventData::setBalance);

    class_<Attachment>("Attachment")
        .property("name", &Attachment::getName);

    // pure_virtual and raw pointer
    class_<VertexAttachment, base<Attachment>>("VertexAttachment")
        .property("id", &VertexAttachment::getId)
        .function("getBones", optional_override([](VertexAttachment &obj){
            return &obj.getBones(); }), allow_raw_pointer<SPVectorSize_t>())
        .function("getVertices", optional_override([](VertexAttachment &obj){
            return &obj.getVertices(); }), allow_raw_pointer<SPVectorFloat>())
        .property("worldVerticesLength", &VertexAttachment::getWorldVerticesLength, &VertexAttachment::setWorldVerticesLength)
        .property("timelineAttachment", &VertexAttachment::getTimelineAttachment, &VertexAttachment::setTimelineAttachment)
        .function("computeWorldVertices", select_overload<void(Slot&, size_t, size_t, Vector<float>&, size_t, size_t)>
        (&VertexAttachment::computeWorldVertices), allow_raw_pointer<SPVectorFloat>())
        .function("copyTo", &VertexAttachment::copyTo, allow_raw_pointers());

    class_<BoundingBoxAttachment, base<VertexAttachment>>("BoundingBoxAttachment")
        .constructor<const String &>()
        .property("name", &BoundingBoxAttachment::getName)
        .function("copy", &BoundingBoxAttachment::copy, allow_raw_pointers());

    class_<ClippingAttachment, base<VertexAttachment>>("ClippingAttachment")
        .constructor<const String &>()
        .property("endSlot", &ClippingAttachment::getEndSlot, &ClippingAttachment::setEndSlot)
        .function("copy", &ClippingAttachment::copy, allow_raw_pointers());

    class_<MeshAttachment, base<VertexAttachment>>("MeshAttachment")
        .constructor<const String &>()
        .property("path", &MeshAttachment::getPath, &MeshAttachment::setPath)
        .function("getRegionUVs", optional_override([](MeshAttachment &obj) {
            return &obj.getRegionUVs(); }), allow_raw_pointer<SPVectorFloat>())
        .function("getUVs", optional_override([](MeshAttachment &obj) { 
            return &obj.getUVs(); }), allow_raw_pointer<SPVectorFloat>())
        .function("getTriangles", optional_override([](MeshAttachment &obj) {
            return &obj.getTriangles(); }), allow_raw_pointer<SPVectorUnsignedShort>())
        .property("color", &MeshAttachment::getColor)
        .property("width", &MeshAttachment::getWidth, &MeshAttachment::setWidth)
        .property("height", &MeshAttachment::getHeight, &MeshAttachment::setHeight)
        .property("hullLength", &MeshAttachment::getHullLength, &MeshAttachment::setHullLength)
        .function("getEdges", optional_override([](MeshAttachment &obj) {
            return &obj.getEdges(); }), allow_raw_pointer<SPVectorUnsignedShort>())
        .function("getRegion", &MeshAttachment::getRegion, allow_raw_pointers())
        .function("setRegion", &MeshAttachment::setRegion, allow_raw_pointers())
        .function("getSequence", &MeshAttachment::getSequence, allow_raw_pointers())
        .function("setSequence", &MeshAttachment::setSequence, allow_raw_pointers())
        .function("updateRegion", &MeshAttachment::updateRegion)
        .function("getParentMesh", &MeshAttachment::getParentMesh, allow_raw_pointers())
        .function("setParentMesh", &MeshAttachment::setParentMesh, allow_raw_pointers())
        .function("computeWorldVertices", select_overload<void(Slot&, size_t, size_t, Vector<float>&, size_t, size_t)>
        (&MeshAttachment::computeWorldVertices), allow_raw_pointer<SPVectorFloat>())
        .function("copy", &MeshAttachment::copy, allow_raw_pointers())
        .function("newLinkedMesh", &MeshAttachment::newLinkedMesh, allow_raw_pointers());

    class_<PathAttachment, base<VertexAttachment>>("PathAttachment")
        .constructor<const String &>()
        .function("getLengths", optional_override([](PathAttachment &obj) {
            return &obj.getLengths(); }), allow_raw_pointer<SPVectorFloat>())
        .property("closed", &PathAttachment::isClosed, &PathAttachment::setClosed)
        .property("constantSpeed", &PathAttachment::isConstantSpeed, &PathAttachment::setConstantSpeed)
        .function("copy", &PathAttachment::copy, allow_raw_pointers());

    class_<PointAttachment, base<Attachment>>("PointAttachment")
        .constructor<const String &>()
        .property("x", &PointAttachment::getX, &PointAttachment::setX)
        .property("y", &PointAttachment::getY, &PointAttachment::setY)
        .property("rotation", &PointAttachment::getRotation, &PointAttachment::setRotation)
        .function("computeWorldPosition", optional_override([](PointAttachment &obj, Bone &bone, float ox, float oy) {
            obj.computeWorldPosition(bone, ox, oy);}))
        .function("computeWorldRotation", &PointAttachment::computeWorldRotation)
        .function("copy", &PointAttachment::copy, allow_raw_pointers());

    class_<RegionAttachment, base<Attachment>>("RegionAttachment")
        .constructor<const String &>()
        .property("x", &RegionAttachment::getX, &RegionAttachment::setX)
        .property("y", &RegionAttachment::getY, &RegionAttachment::setY)
        .property("scaleX", &RegionAttachment::getScaleX, &RegionAttachment::setScaleX)
        .property("scaleY", &RegionAttachment::getScaleY, &RegionAttachment::setScaleY)
        .property("rotation", &RegionAttachment::getRotation, &RegionAttachment::setRotation)
        .property("width", &RegionAttachment::getWidth, &RegionAttachment::setWidth)
        .property("height", &RegionAttachment::getHeight, &RegionAttachment::setHeight)
        .property("color", &RegionAttachment::getColor)
        .property("path", &RegionAttachment::getPath)
        .function("getRegion", &RegionAttachment::getRegion, allow_raw_pointers())
        .function("getOffset", optional_override([](RegionAttachment &obj) {
            return &obj.getOffset(); }), allow_raw_pointer<SPVectorFloat>())
        .function("getUVs", optional_override([](RegionAttachment &obj) {
            return &obj.getUVs(); }), allow_raw_pointer<SPVectorFloat>())
        .function("updateRegion", &RegionAttachment::updateRegion)
        .function("computeWorldVertices", select_overload<void(Slot&, Vector<float>&, size_t, size_t)>
        (&RegionAttachment::computeWorldVertices), allow_raw_pointer<SPVectorFloat>())
        .function("copy", &RegionAttachment::copy, allow_raw_pointer<Attachment>());

    class_<AttachmentLoader>("AttachmentLoader")
        //.constructor<>()
        .function("newClippingAttachment", &AttachmentLoader::newClippingAttachment, pure_virtual(), allow_raw_pointers())
        .function("newPointAttachment", &AttachmentLoader::newPointAttachment, pure_virtual(), allow_raw_pointers())
        .function("newPathAttachment", &AttachmentLoader::newPathAttachment, pure_virtual(), allow_raw_pointers())
        .function("newBoundingBoxAttachment", &AttachmentLoader::newBoundingBoxAttachment, pure_virtual(), allow_raw_pointers())
        .function("newMeshAttachment", &AttachmentLoader::newMeshAttachment, pure_virtual(), allow_raw_pointers())
        .function("newRegionAttachment", &AttachmentLoader::newRegionAttachment, pure_virtual(), allow_raw_pointers());

    class_<AtlasAttachmentLoader, base<AttachmentLoader>>("AtlasAttachmentLoader")
        .constructor<Atlas *>()
        .function("newRegionAttachment", &AtlasAttachmentLoader::newRegionAttachment, allow_raw_pointer<RegionAttachment>())
        .function("newMeshAttachment", &AtlasAttachmentLoader::newMeshAttachment, allow_raw_pointer<MeshAttachment>())
        .function("newBoundingBoxAttachment", &AtlasAttachmentLoader::newBoundingBoxAttachment, allow_raw_pointer<BoundingBoxAttachment>())
        .function("newPathAttachment", &AtlasAttachmentLoader::newPathAttachment, allow_raw_pointer<PathAttachment>())
        .function("newPointAttachment", &AtlasAttachmentLoader::newPointAttachment, allow_raw_pointer<PointAttachment>())
        .function("newClippingAttachment", &AtlasAttachmentLoader::newClippingAttachment, allow_raw_pointer<ClippingAttachment>());

    class_<AtlasPage>("TextureAtlasPage")
        .constructor<const String &>()
        .function("getName", optional_override([] (AtlasPage &obj) { return obj.name; }))
        .property("minFilter", &AtlasPage::minFilter)
        .property("magFilter", &AtlasPage::magFilter)
        .property("uWrap", &AtlasPage::uWrap)
        .property("vWrap", &AtlasPage::vWrap)
        //.property("texture", &AtlasPage::texture) // no texture, use renderer object
        .property("width", &AtlasPage::width)
        .property("height", &AtlasPage::height);

    class_<AtlasRegion>("TextureAtlasRegion")
        //.property("page", &AtlasRegion::page)
        .function("getName", optional_override([] (AtlasRegion &obj) { return obj.name; }))
        .property("x", &AtlasRegion::x)
        .property("y", &AtlasRegion::y)
        .property("index", &AtlasRegion::index);
        //.property("texture", &AtlasRegion::height)

    class_<TextureLoader>("TextureLoader");
        

    class_<Atlas>("TextureAtlas")
        .constructor<const String &, TextureLoader *, bool>()
        .function("findRegion", &Atlas::findRegion, allow_raw_pointers());

    class_<PowInterpolation, base<Interpolation>>("Pow")
        .constructor<int>();


    class_<PowOutInterpolation, base<Interpolation>>("PowOut")
        .constructor<int>();

    class_<SlotData>("SlotData")
        .constructor<int, const String &, BoneData &>()
        .property("index", &SlotData::getIndex)
        .property("name", &SlotData::getName)
        .property("boneData", &SlotData::getBoneData)
        .property("color", &SlotData::getColor)
        .property("darkColor", &SlotData::getDarkColor)
        .property("blendMode", &SlotData::getBlendMode, &SlotData::setBlendMode);

    class_<Updatable>("Updatable")
        .function("update", &Updatable::update, pure_virtual())
        .function("isActive", &Updatable::isActive, pure_virtual())
        .property("active", &Updatable::isActive, &Updatable::setActive)
        ;

    class_<IkConstraint, base<Updatable>>("IkConstraint")
        .constructor<IkConstraintData &, Skeleton &>()
        .property("data", &IkConstraint::getData)
        .function("getBones", optional_override([](IkConstraint &obj) {
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBonePtr>())
        .property("target", &IkConstraint::getTarget)
        .property("bendDirection", &IkConstraint::getBendDirection)
        .property("compress", &IkConstraint::getCompress)
        .property("stretch", &IkConstraint::getStretch)
        .property("mix", &IkConstraint::getMix)
        .property("softness", &IkConstraint::getSoftness)	
        .function("setActive", &IkConstraint::setActive)
        .function("isActive", &IkConstraint::isActive)
		.function("setToSetupPose", &IkConstraint::setToSetupPose)
        .class_function("apply1", select_overload<void (Bone &, float, float, bool, bool, bool, float)>(&IkConstraint::apply))
        .class_function("apply2", select_overload<void (Bone &, Bone &, float, float, int, bool, bool, float, float)>(&IkConstraint::apply))
        ;

    class_<PathConstraint, base<Updatable>>("PathConstraint")
        .constructor<PathConstraintData &, Skeleton &>()
        .property("data", &PathConstraint::getData)
        .function("getBones", optional_override([](PathConstraint &obj) {
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBonePtr>())
        .property("target", &PathConstraint::getTarget)
        .property("position", &PathConstraint::getPosition)
        .property("spacing", &PathConstraint::getSpacing)
        .property("mixRotate", &PathConstraint::getMixRotate)
        .property("mixX", &PathConstraint::getMixX)
		.property("mixY", &PathConstraint::getMixY)
        .function("isActive", &PathConstraint::isActive)
        .function("setActive", &PathConstraint::setActive)
		.function("setToSetupPose", &PathConstraint::setToSetupPose)
        ;

    class_<TransformConstraintData, base<ConstraintData>>("TransformConstraintData")
        .constructor<const String &>()
        .function("getBones", optional_override([](TransformConstraintData &obj) { 
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBoneDataPtr>())
        .property("target", &TransformConstraintData::getTarget)
        .property("mixRotate", &TransformConstraintData::getMixRotate)
        .property("getMixX", &TransformConstraintData::getMixX)
		.property("getMixY", &TransformConstraintData::getMixY)
        .property("mixScaleX", &TransformConstraintData::getMixScaleX)
		.property("mixScaleY", &TransformConstraintData::getMixScaleY)
        .property("mixShearY", &TransformConstraintData::getMixShearY)
        .property("offsetRotation", &TransformConstraintData::getOffsetRotation)
        .property("offsetX", &TransformConstraintData::getOffsetX)
        .property("offsetY", &TransformConstraintData::getOffsetY)
        .property("offsetScaleX", &TransformConstraintData::getOffsetScaleX)
        .property("offsetScaleY", &TransformConstraintData::getOffsetScaleY)
        .property("offsetShearY", &TransformConstraintData::getOffsetShearY)
        .property("relative", &TransformConstraintData::isRelative)
        .property("local", &TransformConstraintData::isLocal);

    class_<TransformConstraint, base<Updatable>>("TransformConstraint")
        .constructor<TransformConstraintData &, Skeleton &>()
        .property("data", &TransformConstraint::getData)
        .function("getBones", optional_override([](TransformConstraint &obj) {
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBonePtr>())
        .property("target", &TransformConstraint::getTarget)
        .property("mixRotate", &TransformConstraint::getMixRotate)
        .property("mixX", &TransformConstraint::getMixX)
		.property("mixY", &TransformConstraint::getMixY)
        .property("mixScaleX", &TransformConstraint::getMixScaleX)
		.property("mixScaleY", &TransformConstraint::getMixScaleY)
        .property("mixShearY", &TransformConstraint::getMixShearY)
		.function("isActive", &PathConstraint::isActive)
        .function("setActive", &PathConstraint::setActive)
		.function("setToSetupPose", &PathConstraint::setToSetupPose)
        ;

    class_<Bone, base<Updatable>>("Bone")
        .constructor<BoneData &, Skeleton &, Bone *>()
        .property("data", &Bone::getData)
        .property("skeleton", &Bone::getSkeleton)
        .property("parent", &Bone::getParent)
        .function("getChildren", optional_override([](Bone &obj) {
            return &obj.getChildren(); }), allow_raw_pointer<SPVectorBonePtr>())
        .property("x", &Bone::getX)
        .property("y", &Bone::getY)
        .property("rotation", &Bone::getRotation)
        .property("scaleX", &Bone::getScaleX)
        .property("scaleY", &Bone::getScaleY)
        .property("shearX", &Bone::getShearX)
        .property("shearY", &Bone::getShearY)
        .property("ax", &Bone::getAX, &Bone::setAX)
        .property("ay", &Bone::getAY, &Bone::setAY)
        .property("appliedRotation", &Bone::getAppliedRotation)
        .property("ascaleX", &Bone::getAScaleX)
        .property("ascaleY", &Bone::getAScaleY)
        .property("ashearX", &Bone::getAShearX)
        .property("ashearY", &Bone::getAShearY)
        .property("a", &Bone::getA)
        .property("b", &Bone::getB)
        .property("c", &Bone::getC)
        .property("d", &Bone::getD)
        .property("worldX", &Bone::getWorldX)
        .property("worldY", &Bone::getWorldY)
        .property("inherit", &Bone::getInherit)        
        .function("updateWorldTransform", select_overload<void()>(&Bone::updateWorldTransform))
        .function("updateWorldTransformWith", select_overload<void(float, float, float, float, float, float, float)>(&Bone::updateWorldTransform))
        .function("updateAppliedTransform", select_overload<void()>(&Bone::updateAppliedTransform))
		.function("setToSetupPose", &Bone::setToSetupPose)
        .function("getWorldRotationX", &Bone::getWorldRotationX)
        .function("getWorldRotationY", &Bone::getWorldRotationY)
        .function("getWorldScaleX", &Bone::getWorldScaleX)
        .function("getWorldScaleY", &Bone::getWorldScaleY)
        .function("worldToLocal", optional_override([](Bone &obj, Vector2 &vec2) {
                float outLocalX, outLocalY;
                obj.worldToLocal(vec2.x, vec2.y, outLocalX, outLocalY);
                vec2.x = outLocalX;
                vec2.y = outLocalY;
            }), 
            allow_raw_pointers()
        )
        .function("localToWorld", optional_override([](Bone &obj, Vector2 &vec2) {
                float outWorldX, outWorldY;
                obj.localToWorld(vec2.x, vec2.y, outWorldX, outWorldY);
                vec2.x = outWorldX;
                vec2.y = outWorldY;
            }), 
            allow_raw_pointers()
        )
		.function("worldToParent", optional_override([](Bone &obj, Vector2 &vec2) {
                float outParentX, outParentY;
                obj.worldToLocal(vec2.x, vec2.y, outParentX, outParentY);
                vec2.x = outParentX;
                vec2.y = outParentY;
            }), 
            allow_raw_pointers()
        )
		.function("parentToWorld", optional_override([](Bone &obj, Vector2 &vec2) {
                float outX, outY;
                obj.worldToLocal(vec2.x, vec2.y, outX, outY);
                vec2.x = outX;
                vec2.y = outY;
            }), 
            allow_raw_pointers()
        )
        .function("worldToLocalRotation", &Bone::worldToLocalRotation)
        .function("localToWorldRotation", &Bone::localToWorldRotation)
        .function("rotateWorld", &Bone::rotateWorld);

    class_<BoneData>("BoneData")
        .constructor<int, const String &, BoneData *>()
        .property("index", &BoneData::getIndex)
        .property("name",  &BoneData::getName) //FIXME(cjh): Don't copy string
        .property("parent", &BoneData::getParent)
        .property("length", &BoneData::getLength, &BoneData::setLength)
        .property("x", &BoneData::getX, &BoneData::setX)
        .property("y", &BoneData::getY, &BoneData::setY)
        .property("rotation", &BoneData::getRotation, &BoneData::setRotation)
        .property("scaleX", &BoneData::getScaleX, &BoneData::setScaleX)
        .property("scaleY", &BoneData::getScaleY, &BoneData::setScaleY)
        .property("shearX", &BoneData::getShearX, &BoneData::setShearX)
        .property("shearY", &BoneData::getShearY, &BoneData::setShearY)
        .property("skinRequired", &BoneData::isSkinRequired, &BoneData::setSkinRequired);


    class_<Slot>("Slot")
        .constructor<SlotData &, Bone &>()
        .property("data", &Slot::getData)
        .property("bone", &Slot::getBone)
        .property("color", &Slot::getColor)
        .property("darkColor", &Slot::getDarkColor)
        .function("getDeform", &Slot::getDeform)
        .function("getSkeleton", &Slot::getSkeleton)
        .function("getAttachment", &Slot::getAttachment, allow_raw_pointers())
        .function("setAttachment", &Slot::setAttachment, allow_raw_pointers())
        .function("getAttachmentState", &Slot::getAttachmentState)
        .function("setAttachmentState", &Slot::setAttachmentState)
        .function("getSequenceIndex", &Slot::getSequenceIndex)
        .function("setSequenceIndex", &Slot::setSequenceIndex)
        .function("setToSetupPose", &Slot::setToSetupPose);

    class_<Skin>("Skin")
        .constructor<const String &>()
        .property("name", &Skin::getName)
        .function("getBones", optional_override([](Skin &obj) {
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBoneDataPtr>())
        .function("getConstraints", optional_override([](Skin &obj) {
            return &obj.getConstraints(); }), allow_raw_pointer<SPVectorConstraintDataPtr>())
        .function("setAttachment", &Skin::setAttachment, allow_raw_pointers())
        .function("addSkin", select_overload<void(Skin *)>(&Skin::addSkin), allow_raw_pointers())
        .function("copySkin", select_overload<void(Skin *)>(&Skin::copySkin), allow_raw_pointers())
        .function("findNamesForSlot", optional_override([](Skin &obj, size_t slotIndex) {
            Vector<String> vetNames;
            auto entries = obj.getAttachments();
            while (entries.hasNext()) {
                auto &entry = entries.next();
                if (entry._slotIndex == slotIndex) vetNames.add(entry._name);
            }
            return vetNames; 
        }), allow_raw_pointers())
        .function("getAttachment", &Skin::getAttachment, allow_raw_pointers())
        .function("getAttachments", optional_override([](Skin &obj) {
            SPVectorSkinEntryPtr entriesVector;
            auto entries = obj.getAttachments();
            while (entries.hasNext()) {
                entriesVector.add(&entries.next());
            }
            return entriesVector;
        }),allow_raw_pointers())
        .function("removeAttachment", &Skin::removeAttachment)
        .function("getAttachmentsForSlot", optional_override([](Skin &obj, size_t index) {
            SPVectorSkinEntryPtr entriesVector;
            auto entries = obj.getAttachments();
            while (entries.hasNext()) {
                Skin::AttachmentMap::Entry &entry = entries.next();
                if (entry._slotIndex == index) entriesVector.add(&entry);
            }
            return entriesVector;
        }),allow_raw_pointers());

    class_<Skin::AttachmentMap::Entry>("SkinEntry")
        .constructor<size_t, const String &, Attachment *>()
        .property("slotIndex", &Skin::AttachmentMap::Entry::_slotIndex)
        .function("getName", optional_override([](Skin::AttachmentMap::Entry& obj) { return obj._name; }))
        .function("getAttachment", optional_override([](Skin::AttachmentMap::Entry &obj) { return obj._attachment; }), allow_raw_pointers());

    class_<SkeletonClipping>("SkeletonClipping")
        .constructor<>()
        .property("clippedVertices", &SkeletonClipping::getClippedVertices)
        .property("clippedTriangles", &SkeletonClipping::getClippedTriangles)
        .property("clippedUVs", &SkeletonClipping::getClippedUVs)
        .function("clipStart", &SkeletonClipping::clipStart, allow_raw_pointers())
        .function("clipEndWithSlot", select_overload<void(Slot &)>(&SkeletonClipping::clipEnd))
        .function("clipEnd", select_overload<void()>(&SkeletonClipping::clipEnd))
        .function("isClipping", &SkeletonClipping::isClipping);

    class_<SkeletonData>("SkeletonData")
        .constructor<>()
        .property("name", &SkeletonData::getName, &SkeletonData::setName)
        .function("getBones", optional_override([](SkeletonData &obj) {
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBoneDataPtr>())
        .function("getSlots", optional_override([](SkeletonData &obj) {
            return &obj.getSlots(); }), allow_raw_pointer<SPVectorSlotDataPtr>())
        .function("getSkins", optional_override([](SkeletonData &obj) {
            return &obj.getSkins(); }), allow_raw_pointer<SPVectorSkinPtr>())
        .property("defaultSkin", &SkeletonData::getDefaultSkin, &SkeletonData::setDefaultSkin)
        .function("getEvents", optional_override([](SkeletonData &obj) {
            return &obj.getEvents(); }), allow_raw_pointer<SPVectorEventDataPtr>())
        .function("getAnimations", optional_override([](SkeletonData &obj) {
            return &obj.getAnimations(); }), allow_raw_pointer<SPVectorAnimationPtr>())
        .function("getIkConstraints", optional_override([](SkeletonData &obj) {
            return &obj.getIkConstraints(); }), allow_raw_pointer<SPVectorIkConstraintDataPtr>())
        .function("getTransformConstraints", optional_override([](SkeletonData &obj) {
            return &obj.getTransformConstraints(); }), allow_raw_pointer<SPVectorTransformConstraintDataPtr>())
        .function("getPathConstraints", optional_override([](SkeletonData &obj) {
            return &obj.getPathConstraints(); }), allow_raw_pointer<SPVectorPathConstraintDataPtr>())
        .property("x", &SkeletonData::getX, &SkeletonData::setX)
        .property("y", &SkeletonData::getY, &SkeletonData::setY)
        .property("width", &SkeletonData::getWidth, &SkeletonData::setWidth)
        .property("height", &SkeletonData::getHeight, &SkeletonData::setHeight)
        .property("version", &SkeletonData::getVersion, &SkeletonData::setVersion)
        .property("hash", &SkeletonData::getHash, &SkeletonData::setHash)
        .property("fps", &SkeletonData::getFps, &SkeletonData::setFps)
        .property("imagesPath", &SkeletonData::getImagesPath, &SkeletonData::setImagesPath)
        .property("audioPath", &SkeletonData::getAudioPath, &SkeletonData::setAudioPath)

        .function("findBone", &SkeletonData::findBone, allow_raw_pointers())
        .function("findSlot", &SkeletonData::findSlot, allow_raw_pointers())
        
        .function("findSkin", &SkeletonData::findSkin, allow_raw_pointers())
        .function("findEvent", &SkeletonData::findEvent, allow_raw_pointers())
        .function("findAnimation", &SkeletonData::findAnimation, allow_raw_pointers())
        .function("findIkConstraint", &SkeletonData::findIkConstraint, allow_raw_pointers())
        .function("findTransformConstraint", &SkeletonData::findTransformConstraint, allow_raw_pointers())
        .function("findPathConstraint", &SkeletonData::findPathConstraint, allow_raw_pointers());

    class_<Animation>("Animation")
        .constructor(optional_override([](const String &name, const emscripten::val &value, float duration) -> Animation* {
            auto length = value["length"].as<uint32_t>();
            Vector<Timeline *> timelines;
            timelines.setSize(length, nullptr);
            for (uint32_t i = 0; i < length; ++i) {
                timelines[i] = value[i].as<Timeline*>(allow_raw_pointers());
            }
            return new Animation(name, timelines, duration);
        }))
        .property("name", &Animation::getName)
        .function("getTimelines", optional_override([](Animation &obj) {
            return &obj.getTimelines(); }), allow_raw_pointer<SPVectorTimelinePtr>())
        .function("hasTimeline", &Animation::hasTimeline)
        .property("duration", &Animation::getDuration, &Animation::setDuration);

    class_<Timeline>("Timeline")
        .function("getPropertyIds", &Timeline::getPropertyIds, pure_virtual());

    class_<CurveTimeline, base<Timeline>>("CurveTimeline")
        .function("getFrameCount", &CurveTimeline::getFrameCount)
        .function("setLinear", &CurveTimeline::setLinear)
        .function("setStepped", &CurveTimeline::setStepped)
        .function("setBezier", &CurveTimeline::setBezier)
        .function("getBezierValue", &CurveTimeline::getBezierValue)
        .function("getCurves", &CurveTimeline::getCurves);
	class_<CurveTimeline1, base<CurveTimeline>>("CurveTimeline1")	    
	    .function("setFrame", &CurveTimeline1::setFrame)
		.function("getCurveValue", &CurveTimeline1::getCurveValue)
		.function("getRelativeValue", &CurveTimeline1::getRelativeValue)
		.function("getScaleValue", &CurveTimeline1::getScaleValue)
		.function("getAbsoluteValue1", select_overload<float (float, float, MixBlend, float, float)>(&CurveTimeline1::getAbsoluteValue))
		.function("getAbsoluteValue2", select_overload<float (float, float, MixBlend, float, float, float)>(&CurveTimeline1::getAbsoluteValue))
				;
	
	class_<CurveTimeline2, base<CurveTimeline>>("CurveTimeline2")
	    .function("setFrame", &CurveTimeline2::setFrame)
		.function("getCurveValue", &CurveTimeline2::getCurveValue);
		
    class_<TranslateTimeline, base<CurveTimeline2>>("TranslateTimeline")
        .constructor<size_t, size_t, int>()
        .function("getBoneIndex", &TranslateTimeline::getBoneIndex)
        .function("setBoneIndex", &TranslateTimeline::setBoneIndex)
        .function("apply", &TranslateTimeline::apply,allow_raw_pointers());

    class_<ScaleTimeline, base<CurveTimeline2>>("ScaleTimeline")
        .constructor<size_t, size_t, int>()
		.function("getBoneIndex", &ScaleTimeline::getBoneIndex)
        .function("setBoneIndex", &ScaleTimeline::setBoneIndex)
        .function("apply", &ScaleTimeline::apply,allow_raw_pointers());

    class_<ShearTimeline, base<CurveTimeline2>>("ShearTimeline")
        .constructor<size_t, size_t, int>()
        .function("getBoneIndex", &ShearTimeline::getBoneIndex)
        .function("setBoneIndex", &ShearTimeline::setBoneIndex)
        .function("apply", &ShearTimeline::apply,allow_raw_pointers());
    class_<RotateTimeline, base<CurveTimeline1>>("RotateTimeline")
        .constructor<size_t, size_t, int>()
        .function("getBoneIndex", &RotateTimeline::getBoneIndex)
        .function("setBoneIndex", &RotateTimeline::setBoneIndex)
        .function("apply", &RotateTimeline::apply,allow_raw_pointers());

    class_<RGBATimeline, base<CurveTimeline>>("ColorTimeline")
        .constructor<size_t, size_t, int>()
        .function("getPropertyIds", &RGBATimeline::getPropertyIds)
        .property("slotIndex", &RGBATimeline::getSlotIndex, &RGBATimeline::setSlotIndex)

        .function("getFrames", GETTER_RVAL_TO_PTR(RGBATimeline, getFrames, SPVectorFloat*), allow_raw_pointer<SPVectorFloat>())
        .function("setFrame", &RGBATimeline::setFrame);

    class_<TwoColorTimeline, base<CurveTimeline>>("TwoColorTimeline")
        .constructor<int, size_t>()
        .function("getPropertyIds", &TwoColorTimeline::getPropertyIds)
        .property("slotIndex", &TwoColorTimeline::getSlotIndex, &TwoColorTimeline::setSlotIndex)
        .function("setFrame", &TwoColorTimeline::setFrame);

    class_<AttachmentTimeline, base<Timeline>>("AttachmentTimeline")
        .constructor<size_t, int>()
        .function("getPropertyIds", &AttachmentTimeline::getPropertyIds)
        .property("slotIndex", &AttachmentTimeline::getSlotIndex, &AttachmentTimeline::setSlotIndex)
        .function("getFrames", GETTER_RVAL_TO_PTR(AttachmentTimeline, getFrames, SPVectorFloat*), allow_raw_pointer<SPVectorFloat>())
        .function("getAttachmentNames", &AttachmentTimeline::getAttachmentNames)
        .function("getFrameCount", &AttachmentTimeline::getFrameCount)
        .function("setFrame", &AttachmentTimeline::setFrame, allow_raw_pointers());

    class_<DeformTimeline, base<CurveTimeline>>("DeformTimeline")
        .constructor<size_t, size_t, int, VertexAttachment*>()
        .function("getPropertyIds", &DeformTimeline::getPropertyIds)
        .property("slotIndex", &DeformTimeline::getSlotIndex, &DeformTimeline::setSlotIndex)
        .property("attachment", &DeformTimeline::getAttachment, &DeformTimeline::setAttachment)
        .function("getFrames", GETTER_RVAL_TO_PTR(DeformTimeline, getFrames, SPVectorFloat*), allow_raw_pointer<SPVectorFloat>())
        .function("getFrameVertices", optional_override([](DeformTimeline &obj) {
            return &obj.getVertices(); }), allow_raw_pointer<SPVectorVectorFloat>())
        .function("setFrame", optional_override([](DeformTimeline &obj, int frameIndex, float time, emscripten::val jsArray){
            unsigned count = jsArray["length"].as<unsigned>();
            Vector<float> spVertices;
            spVertices.setSize(count, 0);
            for (int i = 0; i < count; i++) {
                spVertices[i] = jsArray[i].as<float>();
            }
            obj.setFrame(frameIndex, time, spVertices);
        }), allow_raw_pointers());

    class_<EventTimeline, base<Timeline>>("EventTimeline")
        .constructor<int>()
		.function("getPropertyIds", &EventTimeline::getPropertyIds)
        .function("getFrames", GETTER_RVAL_TO_PTR(EventTimeline, getFrames, SPVectorFloat*), allow_raw_pointer<SPVectorFloat>())
        .function("getEvents",  optional_override([](EventTimeline &obj) {
            return &obj.getEvents(); }), allow_raw_pointer<SPVectorEventPtr>())
        .function("getFrameCount", &EventTimeline::getFrameCount)
        .function("setFrame", &EventTimeline::setFrame, allow_raw_pointers());

    class_<DrawOrderTimeline, base<Timeline>>("DrawOrderTimeline")
        .constructor<int>()
		.function("getPropertyIds", &DrawOrderTimeline::getPropertyIds)
        .function("getFrames", GETTER_RVAL_TO_PTR(DrawOrderTimeline, getFrames, SPVectorFloat*), allow_raw_pointer<SPVectorFloat>())
        .function("getFrameCount", &DrawOrderTimeline::getFrameCount)
        .function("getDrawOrders", optional_override([](DrawOrderTimeline &obj) { 
            return &obj.getDrawOrders(); }), allow_raw_pointer<SPVectorVectorInt>())
        .function("setFrame", &DrawOrderTimeline::setFrame, allow_raw_pointers());

    class_<IkConstraintTimeline, base<CurveTimeline>>("IkConstraintTimeline")
        .constructor<size_t, size_t,int>()
        .function("getPropertyIds", &IkConstraintTimeline::getPropertyIds)
        .function("setFrame", &IkConstraintTimeline::setFrame);

    class_<TransformConstraintTimeline, base<CurveTimeline>>("TransformConstraintTimeline")
        .constructor<size_t,size_t,int>()
        .function("getPropertyIds", &TransformConstraintTimeline::getPropertyIds)
        .function("setFrame", &TransformConstraintTimeline::setFrame);

    class_<PathConstraintPositionTimeline, base<CurveTimeline>>("PathConstraintPositionTimeline")
        .constructor<size_t,size_t,int>()
        .function("getPropertyIds", &PathConstraintPositionTimeline::getPropertyIds)
        .function("setFrame", &PathConstraintPositionTimeline::setFrame);

    class_<PathConstraintMixTimeline, base<CurveTimeline>>("PathConstraintMixTimeline")
        .constructor<size_t,size_t,int>()
		.function("getPropertyIds", &PathConstraintMixTimeline::getPropertyIds)
        ;

    class_<TrackEntry>("TrackEntry")
        .constructor<>()
        .property("animation", &TrackEntry::getAnimation)
        .property("next", &TrackEntry::getNext)
        .property("mixingFrom", &TrackEntry::getMixingFrom)
        .property("mixingTo", &TrackEntry::getMixingTo)
        //.function("getProp_listener", &TrackEntry::listener)
        .property("trackIndex", &TrackEntry::getTrackIndex)
        .property("loop", &TrackEntry::getLoop, &TrackEntry::setLoop)
        .property("holdPrevious", &TrackEntry::getHoldPrevious, &TrackEntry::setHoldPrevious)
        .property("eventThreshold", &TrackEntry::getEventThreshold, &TrackEntry::setEventThreshold)
        .property("mixAttachmentThreshold", &TrackEntry::getMixAttachmentThreshold, &TrackEntry::setMixAttachmentThreshold)
        .property("alphaAttachmentThreshold", &TrackEntry::getAlphaAttachmentThreshold, &TrackEntry::setAlphaAttachmentThreshold)
        .property("mixDrawOrderThreshold", &TrackEntry::getMixDrawOrderThreshold, &TrackEntry::setMixDrawOrderThreshold)
        .property("animationStart", &TrackEntry::getAnimationStart, &TrackEntry::setAnimationStart)
        .property("animationEnd", &TrackEntry::getAnimationEnd, &TrackEntry::setAnimationEnd)
        .property("animationLast", &TrackEntry::getAnimationLast, &TrackEntry::setAnimationLast)
        .property("delay", &TrackEntry::getDelay, &TrackEntry::setDelay)
        .property("trackTime", &TrackEntry::getTrackTime, &TrackEntry::setTrackTime)
        .property("trackEnd", &TrackEntry::getTrackEnd, &TrackEntry::setTrackEnd)
        .property("timeScale", &TrackEntry::getTimeScale, &TrackEntry::setTimeScale)
        .property("alpha", &TrackEntry::getAlpha, &TrackEntry::setAlpha)
        .property("mixTime", &TrackEntry::getMixTime, &TrackEntry::setMixTime)
        .property("mixBlend", &TrackEntry::getMixBlend, &TrackEntry::setMixBlend)
        .function("setMixDuration1", select_overload<void(float)>(&TrackEntry::setMixDuration))
        .function("setMixDuration2", select_overload<void(float,float)>(&TrackEntry::setMixDuration))
        .function("getMixDuration", &TrackEntry::getMixDuration)
        .function("getAnimationTime", &TrackEntry::getAnimationTime)
        .function("isComplete", &TrackEntry::isComplete)
        .function("resetRotationDirections", &TrackEntry::resetRotationDirections);

    class_<AnimationStateData>("AnimationStateData")
        .constructor<SkeletonData *>()
        .property("defaultMix", &AnimationStateData::getDefaultMix, &AnimationStateData::setDefaultMix)
        .property("skeletonData", &AnimationStateData::getSkeletonData)
        .function("setMix", select_overload<void(const String&, const String&, float)>(&AnimationStateData::setMix), allow_raw_pointers())
        .function("setMixWith", select_overload<void (Animation*, Animation* , float)>(&AnimationStateData::setMix), allow_raw_pointers())
        .function("getMix", &AnimationStateData::getMix, allow_raw_pointers());

    class_<AnimationState>("AnimationState")
        .constructor<AnimationStateData *>()
        .property("data", &AnimationState::getData)
        .function("getTracks", optional_override([](AnimationState &obj) {
            return &obj.getTracks(); }), allow_raw_pointer<SPVectorTrackEntryPtr>())
        .property("timeScale", &AnimationState::getTimeScale, &AnimationState::setTimeScale)
        .function("update", &AnimationState::update)
        .function("apply", &AnimationState::apply)
        .function("clearTracks", &AnimationState::clearTracks)
        .function("clearTrack", &AnimationState::clearTrack)
        .function("setAnimation", select_overload<TrackEntry* (size_t, const String&, bool)>(&AnimationState::setAnimation), allow_raw_pointers())
        .function("setAnimationWith", optional_override([](AnimationState &obj, uint32_t trackIndex, Animation *animation, bool loop) { return obj.setAnimation(trackIndex, animation, loop); }), allow_raw_pointers())
        .function("addAnimation", select_overload<TrackEntry* (size_t, const String&, bool, float)>(&AnimationState::addAnimation), allow_raw_pointers())
        .function("addAnimationWith", select_overload<TrackEntry* (size_t, Animation *animation, bool, float)>(&AnimationState::addAnimation), allow_raw_pointers())
        .function("setEmptyAnimation", &AnimationState::setEmptyAnimation, allow_raw_pointers())
        .function("addEmptyAnimation", &AnimationState::addEmptyAnimation, allow_raw_pointers())
        .function("setEmptyAnimations", &AnimationState::setEmptyAnimations)
        .function("getCurrent", &AnimationState::getCurrent, allow_raw_pointer<TrackEntry>())
        // .function("setListener",  optional_override([](AnimationState &obj, AnimationStateListener inValue) {
        //     obj.setListener(inValue); }),allow_raw_pointers())
        // .function("setListenerObject", optional_override([](AnimationState &obj, AnimationStateListenerObject *inValue) {
        //     obj.setListener(inValue); }),allow_raw_pointers())
        .function("disableQueue", &AnimationState::disableQueue)
        .function("enableQueue", &AnimationState::enableQueue);
        //.function("addListener", &AnimationState::addListener)
        //.function("removeListener", &AnimationState::removeListener)
        //.function("clearListeners", &AnimationState::clearListeners) // no have clearListeners

    //private
    // class_<EventQueue>("EventQueue")
    //     .constructor<AnimationState& , Pool<TrackEntry>& >()
    //     .function("start", &EventQueue::start, allow_raw_pointers())
    //     .function("interrupt", &EventQueue::interrupt, allow_raw_pointers())
    //     .function("end", &EventQueue::end, allow_raw_pointers())
    //     .function("dispose", &EventQueue::dispose, allow_raw_pointers())
    //     .function("complete", &EventQueue::complete, allow_raw_pointers())
    //     .function("event", &EventQueue::event, allow_raw_pointers())
    //     .function("drain", &EventQueue::drain)
    //     .function("clear");

    //class_<AnimationStateListener>("AnimationStateListener")

    //class_<AnimationStateListenerObject>("AnimationStateListenerObject")
    //    .constructor<>()
    //    .function("callback", &AnimationStateListenerObject::callback, pure_virtual());

    //class_<AnimationStateAdapter>("AnimationStateAdapter")

    class_<Skeleton>("Skeleton")
        .constructor<SkeletonData *>()
        .property("data", &Skeleton::getData)
        .function("getBones", optional_override([](Skeleton &obj){
            return &obj.getBones(); }), allow_raw_pointer<SPVectorBonePtr>())
        .function("getSlots", optional_override([](Skeleton &obj){ 
            return &obj.getSlots(); }), allow_raw_pointer<SPVectorSlotPtr>())
        .function("getDrawOrder", optional_override([](Skeleton &obj){
            return &obj.getDrawOrder(); }), allow_raw_pointer<SPVectorSlotPtr>())
        .function("getIkConstraints", optional_override([](Skeleton &obj){
            return &obj.getIkConstraints(); }), allow_raw_pointer<SPVectorIkConstraintPtr>())
        .function("getTransformConstraints", optional_override([](Skeleton &obj){
            return &obj.getTransformConstraints(); }), allow_raw_pointer<SPVectorTransformConstraintPtr>())
        .function("getPathConstraints", optional_override([](Skeleton &obj){
            return &obj.getPathConstraints(); }), allow_raw_pointer<SPVectorPathConstraintPtr>())
        .function("getUpdateCacheList", optional_override([](Skeleton &obj){
            return &obj.getUpdateCacheList(); }), allow_raw_pointer<SPVectorUpdatablePtr>())
        .property("skin", &Skeleton::getSkin)
        .property("color", &Skeleton::getColor)
        .property("time", &Skeleton::getTime, &Skeleton::setTime)
        .property("scaleX", &Skeleton::getScaleX, &Skeleton::setScaleX)
        .property("scaleY", &Skeleton::getScaleY, &Skeleton::setScaleY)
        .property("x", &Skeleton::getX, &Skeleton::setX)
        .property("y", &Skeleton::getY, &Skeleton::setY)
        
        .function("updateCache", &Skeleton::updateCache)
        .function("updateWorldTransform", select_overload<void(Physics)>(&Skeleton::updateWorldTransform))
        .function("updateWorldTransformWithBone", select_overload<void(Physics,Bone*)>(&Skeleton::updateWorldTransform))
        .function("setToSetupPose", &Skeleton::setToSetupPose)
        .function("setBonesToSetupPose", &Skeleton::setBonesToSetupPose)
        .function("setSlotsToSetupPose", &Skeleton::setSlotsToSetupPose)
        .function("getRootBone", &Skeleton::getRootBone, allow_raw_pointer<Bone>())
        .function("findBone", &Skeleton::findBone, allow_raw_pointers())
        .function("findSlot", &Skeleton::findSlot, allow_raw_pointers())
        .function("setSkinByName", select_overload<void(const String &)>(&Skeleton::setSkin))
        .function("setSkin", static_cast<void (Skeleton::*)(Skin *)>(&Skeleton::setSkin), allow_raw_pointer<Skin>())
        .function("getAttachmentByName", select_overload<Attachment*(const String &, const String &)>(&Skeleton::getAttachment), allow_raw_pointers())
        .function("getAttachment", select_overload<Attachment*(int, const String &)>(&Skeleton::getAttachment),allow_raw_pointers())
        .function("setAttachment", &Skeleton::setAttachment)
        .function("findIkConstraint", &Skeleton::findIkConstraint, allow_raw_pointers())
        .function("findTransformConstraint", &Skeleton::findTransformConstraint, allow_raw_pointers())
        .function("findPathConstraint", &Skeleton::findPathConstraint, allow_raw_pointers())
        //.function("getBounds", optional_override([](Skeleton &obj, &outX, ) {}), allow_raw_pointers())
        .function("update", &Skeleton::update);

    //incomplete
    // class_<SkeletonBinary>("SkeletonBinary")
    //     .constructor<Atlas*>()
    //     .constructor<AttachmentLoader*>()
    // .function("setScale", &SkeletonBinary::setScale)
    // .function("getError", &SkeletonBinary::getError);
    //.function("readSkeletonDataFile", optional_override([](SkeletonBinary &obj, const String& path) { return obj.readSkeletonDataFile(path); }));

    // incomplete
    //class_<SkeletonJson>("SkeletonJson")
    //.constructor<Atlas*>()
    //.constructor<AttachmentLoader*>()
    //.function("setScale", &SkeletonJson::setScale);
    //.function("getError", &SkeletonJson::getError);

    class_<SpineModel>("SpineModel")
        .property("vCount", &SpineModel::vCount)
        .property("iCount", &SpineModel::iCount)
        .property("vPtr", &SpineModel::vPtr)
        .property("iPtr", &SpineModel::iPtr)
        .function("getData", &SpineModel::getData, allow_raw_pointer<SPVectorUint>());

    class_<SpineDebugShape>("SpineDebugShape")
        .property("type", &SpineDebugShape::type)
        .property("vOffset", &SpineDebugShape::vOffset)
        .property("vCount", &SpineDebugShape::vCount)
        .property("iOffset", &SpineDebugShape::iOffset)
        .property("iCount", &SpineDebugShape::iCount);

    class_<SpineSkeletonInstance>("SkeletonInstance")
        .constructor<>()
        .property("isCache", &SpineSkeletonInstance::isCache)
        .property("dtRate", &SpineSkeletonInstance::dtRate)
        .property("enable", &SpineSkeletonInstance::enable)
        .function("initSkeleton", &SpineSkeletonInstance::initSkeleton, allow_raw_pointers())
        .function("setAnimation", &SpineSkeletonInstance::setAnimation, allow_raw_pointers())
        .function("setSkin", &SpineSkeletonInstance::setSkin)
        .function("updateAnimation", &SpineSkeletonInstance::updateAnimation)
        .function("updateRenderData", &SpineSkeletonInstance::updateRenderData, allow_raw_pointer<SpineModel>())
        .function("setPremultipliedAlpha", &SpineSkeletonInstance::setPremultipliedAlpha)
        .function("setUseTint", &SpineSkeletonInstance::setUseTint)
        .function("setColor", &SpineSkeletonInstance::setColor)
        .function("getAnimationState", &SpineSkeletonInstance::getAnimationState, allow_raw_pointer<AnimationState>())
        .function("setMix", &SpineSkeletonInstance::setMix)
        .function("setListener", &SpineSkeletonInstance::setListener)
        .function("setTrackEntryListener", &SpineSkeletonInstance::setTrackEntryListener, allow_raw_pointer<TrackEntry *>())
        .function("setDebugMode", &SpineSkeletonInstance::setDebugMode)
        .function("getDebugShapes", GETTER_RVAL_TO_PTR(SpineSkeletonInstance, getDebugShapes, SPVectorDebugShape*), allow_raw_pointers())
        .function("resizeSlotRegion", &SpineSkeletonInstance::resizeSlotRegion)
        .function("destroy", &SpineSkeletonInstance::destroy)
        .function("setSlotTexture", &SpineSkeletonInstance::setSlotTexture);
}

EMSCRIPTEN_BINDINGS(cocos_spine) {
    using namespace emscripten;
    class_<SpineWasmUtil>("SpineWasmUtil")
    .class_function("spineWasmInit", &SpineWasmUtil::spineWasmInit)
    .class_function("spineWasmDestroy", &SpineWasmUtil::spineWasmDestroy)
    .class_function("queryStoreMemory", &SpineWasmUtil::queryStoreMemory)
    .class_function("querySpineSkeletonDataByUUID", &SpineWasmUtil::querySpineSkeletonDataByUUID, allow_raw_pointers())
    .class_function("createSpineSkeletonDataWithJson", &SpineWasmUtil::createSpineSkeletonDataWithJson, allow_raw_pointers())
    .class_function("createSpineSkeletonDataWithBinary", &SpineWasmUtil::createSpineSkeletonDataWithBinary, allow_raw_pointers())
    .class_function("registerSpineSkeletonDataWithUUID", &SpineWasmUtil::registerSpineSkeletonDataWithUUID, allow_raw_pointers())
    .class_function("destroySpineSkeletonDataWithUUID", &SpineWasmUtil::destroySpineSkeletonDataWithUUID)
    .class_function("destroySpineSkeleton", &SpineWasmUtil::destroySpineSkeleton, allow_raw_pointers())
    .class_function("getCurrentListenerID", &SpineWasmUtil::getCurrentListenerID)
    .class_function("getCurrentEventType", &SpineWasmUtil::getCurrentEventType)
    .class_function("getCurrentTrackEntry", &SpineWasmUtil::getCurrentTrackEntry, allow_raw_pointers())
    .class_function("getCurrentEvent", &SpineWasmUtil::getCurrentEvent, allow_raw_pointers());
}
