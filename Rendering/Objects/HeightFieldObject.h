class HeightFieldObject : public ISceneObject {
    std::unique_ptr<HeightFieldMesh> m_mesh;
    const rp3d::ProxyShape* m_shape;
    const rp3d::HeightFieldShape* m_map;
    GMaterial m_material;
public:
    HeightFieldObject(const ResourceRepo* repo, const rp3d::ProxyShape* shape, const rp3d::HeightFieldShape* map, float* data)
    {
        m_shape = shape;
        m_map = map;
        rp3d::Vector3 min, max;
        map->getLocalBounds(min, max);
        m_mesh = std::make_unique<HeightFieldMesh>(data, map->getNbRows(), map->getNbColumns(), max.y - min.y, repo->getGBufferShader());
        m_material = GMaterial(
                repo->getGrassTexture(),
                createTextureColor(0, 0.0, 0));
      }

    void draw(const Camera::Camera* camera) const override {
        m_mesh->draw(camera, getMvp(m_shape), m_material);
    }
};
