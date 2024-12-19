#include "AtlasAttachmentLoaderExtension.h"
#include "mesh-type-define.h"
//#include "LogUtil.h"
using namespace spine;

static uint16_t quadTriangles[6] = {0, 1, 2, 2, 3, 0};

AttachmentVertices::AttachmentVertices(int verticesCount, uint16_t *triangles, int trianglesCount, uint32_t textureId) {
    _triangles = new Triangles();
    _triangles->verts = new V3F_T2F_C4B[verticesCount];
    _triangles->vertCount = verticesCount;
    _triangles->indices = triangles;
    _triangles->indexCount = trianglesCount;
    _textureId = textureId;
}

AttachmentVertices::~AttachmentVertices() {
    delete[] _triangles->verts;
    delete _triangles;
}

AttachmentVertices *AttachmentVertices::copy() {
    AttachmentVertices *atv = new AttachmentVertices(_triangles->vertCount, _triangles->indices, _triangles->indexCount, _textureId);
    return atv;
}

static void deleteAttachmentVertices(void *vertices) {
    delete static_cast<AttachmentVertices *>(vertices);
}

AtlasAttachmentLoaderExtension::AtlasAttachmentLoaderExtension(Atlas *atlas) : AtlasAttachmentLoader(atlas), _atlasCache(atlas) {
}

AtlasAttachmentLoaderExtension::~AtlasAttachmentLoaderExtension() = default;

void AtlasAttachmentLoaderExtension::configureAttachment(Attachment *attachment) {
    if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) {
        auto *regionAttachment = static_cast<RegionAttachment *>(attachment);
        auto &pages = _atlasCache->getPages();
        auto *region = regionAttachment->getRegion();
        if(region){
            // 保证 region->rendererObject 存在并指向有效对象
            AtlasRegion *atlasRegion = static_cast<AtlasRegion *>(region->rendererObject);
			if(atlasRegion)
			{
	            // 初始化 AttachmentVertices 并关联
		        auto *attachmentVertices = new AttachmentVertices(4, quadTriangles, 6, pages.indexOf(atlasRegion->page));
		        V3F_T2F_C4B *vertices = attachmentVertices->_triangles->verts;
		        auto &uvs = regionAttachment->getUVs();
		        for (int i = 0, ii = 0; i < 4; ++i, ii += 2) {
		            vertices[i].texCoord.u = uvs[ii];
		            vertices[i].texCoord.v = uvs[ii + 1];
		        }
		        // 关联 attachmentVertices 到 region 的 rendererObject
	            region->rendererObject = attachmentVertices;
            }
        }
    } else if (attachment->getRTTI().isExactly(MeshAttachment::rtti)) {
        auto *meshAttachment = static_cast<MeshAttachment *>(attachment);
        auto &pages = _atlasCache->getPages();
        auto *region = meshAttachment->getRegion();
        if(region){
            // 保证 region->rendererObject 存在并指向有效对象
            AtlasRegion *atlasRegion = static_cast<AtlasRegion *>(region->rendererObject);
            auto *attachmentVertices = new AttachmentVertices(
            static_cast<int32_t>(meshAttachment->getWorldVerticesLength() >> 1), meshAttachment->getTriangles().buffer(), static_cast<int32_t>(meshAttachment->getTriangles().size()), pages.indexOf(atlasRegion->page));
            V3F_T2F_C4B *vertices = attachmentVertices->_triangles->verts;
            for (size_t i = 0, ii = 0, nn = meshAttachment->getWorldVerticesLength(); ii < nn; ++i, ii += 2) {
                vertices[i].texCoord.u = meshAttachment->getUVs()[ii];
                vertices[i].texCoord.v = meshAttachment->getUVs()[ii + 1];
            }
            // 关联 attachmentVertices 到 region 的 rendererObject
            region->rendererObject = attachmentVertices;
        }
    }
}