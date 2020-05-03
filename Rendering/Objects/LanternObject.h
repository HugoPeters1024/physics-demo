class LanternObject : public ISceneObject {
private:
    const GenericMesh* m_mesh;
    const rp3d::ProxyShape* m_shape;
    const rp3d::BoxShape* m_box;
    GMaterial m_texture;
    Light m_light;
public:
    LanternObject(const ResourceRepo* repo, const rp3d::ProxyShape* shape, const rp3d::BoxShape* box)
    {
      m_mesh = repo->getLampMesh();
      m_shape = shape;
      m_box = box;
      m_texture = GMaterial(
              repo->getBlueTexture(),
              repo->getBlackTexture());
      m_light.color = Vector3(1) * 1000;
    }

    void draw(const Camera::Camera* camera) const override {
      m_mesh->draw(camera, getMvp(m_shape), m_texture);
    }

    std::vector<Light*> getLights() override {
      std::vector<Light*> ret;
      rp3d::Transform offset(rp3d::Vector3(0,1,0), rp3d::Quaternion().identity());
      auto global = m_shape->getLocalToWorldTransform();
      auto transform = global * offset;
      m_light.position = transform.getPosition();
      ret.push_back(&m_light);
      return ret;
    }

    Light* getLight() {
      return &m_light;
    }
};

