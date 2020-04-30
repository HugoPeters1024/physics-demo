class ResourceRepo
    {
    private:
        Logger m_logger = Logger("ResourceRepo");
        std::shared_ptr<GenericMesh> m_cube;
        std::shared_ptr<GenericMesh> m_sphere;
        std::shared_ptr<SkyboxMesh> m_skybox;
        std::shared_ptr<QuadMesh> m_quad;
        std::shared_ptr<LightingQuadMesh> m_lighting_quad;
        std::shared_ptr<VolumeMesh> m_volume_mesh;
        std::shared_ptr<DefaultShader> m_default_shader;
        std::shared_ptr<QuadShader> m_quad_shader;
        std::shared_ptr<GBufferShader> m_gbuffer_shader;
        std::shared_ptr<ShaderSkyBox> m_gbuffer_skybox_shader;
        std::shared_ptr<LightingShader> m_lighting_shader;
        GLuint m_tex_white;
        GLuint m_tex_blue;
        GLuint m_tex_grass;
        GLuint m_tex_mars;
        GLuint m_tex_skybox;
public:
        ResourceRepo(){
          m_logger.logDebug("Compiling default shader");
          m_default_shader = std::make_shared<DefaultShader>();

          m_logger.logDebug("Compiling quad shader");
          m_quad_shader = std::make_shared<QuadShader>();

          m_logger.logDebug("Compiling gbuffer shader");
          m_gbuffer_shader = std::make_shared<GBufferShader>();

          m_logger.logDebug("Compiling gbuffer skybox shader");
          m_gbuffer_skybox_shader = std::make_shared<ShaderSkyBox>();

          m_logger.logDebug("Compiling lighting shader");
          m_lighting_shader = std::make_shared<LightingShader>();

          m_logger.logDebug("Building cube mesh");
          m_cube = std::make_shared<GenericMesh>("Models/cube.obj", getGBufferShader());

          m_logger.logDebug("Building skybox mesh");
          m_skybox = std::make_shared<SkyboxMesh>("Models/cube.obj", getGBufferShaderSkybox());

          m_logger.logDebug("Building sphere mesh");
          m_sphere = std::make_shared<GenericMesh>("Models/sphere.obj", getGBufferShader());

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
          m_tex_mars = loadTexture("Textures/mars.jpg");

          m_logger.logDebug("Generating skybox");
          std::vector<std::string> skybox_faces
          {
            "Textures/skybox/right.jpg",
            "Textures/skybox/left.jpg",
            "Textures/skybox/top.jpg",
            "Textures/skybox/bottom.jpg",
            "Textures/skybox/front.jpg",
            "Textures/skybox/back.jpg",
          };

          m_tex_skybox = loadCubemap(skybox_faces);
        };
        inline const GenericMesh* getCubeMesh() const { return m_cube.get(); }
        inline const GenericMesh* getSphereMesh() const { return m_sphere.get(); }
        inline const SkyboxMesh* getSkyboxMesh() const { return m_skybox.get(); }
        inline const QuadMesh* getQuadMesh() const { return m_quad.get(); }
        inline const LightingQuadMesh* getLightingQuadMesh() const { return m_lighting_quad.get(); }
        inline const VolumeMesh* getVolumeMesh() const { return m_volume_mesh.get(); }
        inline const DefaultShader* getDefaultShader() const { return m_default_shader.get(); }
        inline const QuadShader* getQuadShader() const { return m_quad_shader.get(); }
        inline const GBufferShader* getGBufferShader() const { return m_gbuffer_shader.get(); }
        inline const ShaderSkyBox* getGBufferShaderSkybox() const { return m_gbuffer_skybox_shader.get(); }
        inline const LightingShader* getLightingShader() const { return m_lighting_shader.get(); }
        inline const GLuint getWhiteTexture() const { return m_tex_white; }
        inline const GLuint getBlueTexture() const { return m_tex_blue; }
        inline const GLuint getGrassTexture() const { return m_tex_grass; }
        inline const GLuint getMarsTexture() const { return m_tex_mars; }
        inline const GLuint getSkyBoxTexture() const { return m_tex_skybox; }

};
