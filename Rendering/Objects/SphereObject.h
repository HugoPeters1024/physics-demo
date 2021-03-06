class SphereObject : public ISceneObject {
private:
    const GenericMesh* m_mesh;
    const rp3d::ProxyShape* m_shape;
    const rp3d::SphereShape* m_sphere;
    GMaterial m_material;
public:
    SphereObject(const ResourceRepo* repo, const rp3d::ProxyShape* shape, const rp3d::SphereShape* sphere)
    {
      m_mesh = repo->getSphereMesh();
      m_shape = shape;
      m_sphere = sphere;
      m_material = GMaterial(
              repo->getMarsTexture(),
              repo->getLightingTexture());
    }

    void draw(const Camera::Camera* camera) const override {
      m_mesh->draw(camera, getMvp(m_shape) * Matrix4::FromScale(Vector3(m_sphere->getRadius()/18.737)), m_material);
    }
};

