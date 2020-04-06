class RayCastCacher : public rp3d::RaycastCallback
{
public:
    rp3d::RaycastInfo m_info;
    Camera::Camera* m_camera;
    RayCastCacher(Camera::Camera* camera)
    {
      m_camera = camera;
    }

    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override {
      if (m_camera->getPosition().y < info.worldPoint.y + 2)
      {
        m_camera->pos.y = info.worldPoint.y + 2;
        m_camera->velocity = Vector3(0);
      }

      return m_info.hitFraction;
    }

};