class ISceneObject {
protected:
    inline virtual Matrix4 getMvp(const rp3d::ProxyShape* shape) const
    {
      float mvp[16];
      shape->getLocalToWorldTransform().getOpenGLMatrix(mvp);
      return Matrix4::FromArray(mvp);
    }
public:
    virtual void draw(const Camera::Camera* camera) const = 0;
};

