class HeightFieldObject : public ISceneObject {
    std::unique_ptr<HeightFieldMesh> m_mesh;
    const rp3d::ProxyShape* m_shape;
    const rp3d::HeightFieldShape* m_map;
    GLuint m_texture;
public:
    HeightFieldObject(const ResourceRepo* repo, const rp3d::ProxyShape* shape, const rp3d::HeightFieldShape* map, float* data)
    {
        m_shape = shape;
        m_map = map;
        rp3d::Vector3 min, max;
        map->getLocalBounds(min, max);
        m_mesh = std::make_unique<HeightFieldMesh>(data, map->getNbRows(), map->getNbColumns(), max.y - min.y, repo->getGBufferShader());
        m_texture = repo->getGrassTexture();
      }

    void draw(const Camera::Camera* camera) const override {
        m_mesh->draw(camera, getMvp(m_shape) * Matrix4::FromScale(Vector3(m_map->getScaling())), m_texture);
    }
};
