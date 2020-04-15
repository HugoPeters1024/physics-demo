class ResourceRepo
    {
    private:
        Logger m_logger = Logger("ResourceRepo");
        std::shared_ptr<CubeMesh> m_cube;
        std::shared_ptr<SphereMesh> m_sphere;
        std::shared_ptr<QuadMesh> m_quad;
        std::shared_ptr<LightingQuadMesh> m_lighting_quad;
        std::shared_ptr<VolumeMesh> m_volume_mesh;
        std::shared_ptr<DefaultShader> m_default_shader;
        std::shared_ptr<QuadShader> m_quad_shader;
        std::shared_ptr<GBufferShader> m_gbuffer_shader;
        std::shared_ptr<LightingShader> m_lighting_shader;
        GLuint m_tex_white;
        GLuint m_tex_blue;
        GLuint m_tex_grass;
public:
        ResourceRepo(){
          m_logger.logDebug("Compiling default shader");
          m_default_shader = std::make_shared<DefaultShader>();

          m_logger.logDebug("Compiling quad shader");
          m_quad_shader = std::make_shared<QuadShader>();

          m_logger.logDebug("Compiling gbuffer shader");
          m_gbuffer_shader = std::make_shared<GBufferShader>();

          m_logger.logDebug("Compiling lighting shader");
          m_lighting_shader = std::make_shared<LightingShader>();

          m_logger.logDebug("Building cube mesh");
          m_cube = std::make_shared<CubeMesh>(getGBufferShader());

          m_logger.logDebug("Building sphere mesh");
          m_sphere = std::make_shared<SphereMesh>(getGBufferShader());

          m_logger.logDebug("Building quad mesh");
          m_quad = std::make_shared<QuadMesh>(getQuadShader());

          m_logger.logDebug("Building lighting quad mesh");
          m_lighting_quad = std::make_shared<LightingQuadMesh>(getLightingShader());

          m_logger.logDebug("Building volume quad mesh");
          m_volume_mesh = std::make_shared<VolumeMesh>(getLightingShader());

          m_logger.logDebug("Generating textures");
          m_tex_white = createTextureColor(1, 1, 1);
          m_tex_blue = createTextureColor(0.2f, 0.2f, 1.0f);
          m_tex_grass = loadTexture("Textures/grass.jpg");
        };
        inline const CubeMesh* getCubeMesh() const { return m_cube.get(); }
        inline const SphereMesh* getSphereMesh() const { return m_sphere.get(); }
        inline const QuadMesh* getQuadMesh() const { return m_quad.get(); }
        inline const LightingQuadMesh* getLightingQuadMesh() const { return m_lighting_quad.get(); }
        inline const VolumeMesh* getVolumeMesh() const { return m_volume_mesh.get(); }
        inline const DefaultShader* getDefaultShader() const { return m_default_shader.get(); }
        inline const QuadShader* getQuadShader() const { return m_quad_shader.get(); }
        inline const GBufferShader* getGBufferShader() const { return m_gbuffer_shader.get(); }
        inline const LightingShader* getLightingShader() const { return m_lighting_shader.get(); }
        inline const GLuint getWhiteTexture() const { return m_tex_white; }
        inline const GLuint getBlueTexture() const { return m_tex_blue; }
        inline const GLuint getGrassTexture() const { return m_tex_grass; }

};
