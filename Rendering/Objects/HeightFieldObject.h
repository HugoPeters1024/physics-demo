class HeightFieldObject : public ISceneObject {
    std::unique_ptr<HeightFieldMesh> m_mesh;
    const rp3d::ProxyShape* m_shape;
    const rp3d::HeightFieldShape* m_map;
public:
    HeightFieldObject(const rp3d::ProxyShape* shape, const rp3d::HeightFieldShape* map, float* data)
    {
       m_shape = shape;
        m_map = map;
        rp3d::Vector3 min, max;
        map->getLocalBounds(min, max);
        m_mesh = std::make_unique<HeightFieldMesh>(data, map->getNbRows(), map->getNbColumns(), max.y - min.y);
      }

    void draw() const override {
        m_mesh->draw(getMvp(m_shape) * Matrix4::FromScale(Vector3(m_map->getScaling())));
    }
};
