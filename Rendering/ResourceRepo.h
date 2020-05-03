class ResourceRepo
    {
    private:
        Logger m_logger = Logger("ResourceRepo");
        std::shared_ptr<GenericMesh> m_cube;
        std::shared_ptr<GenericMesh> m_sphere;
        std::shared_ptr<GenericMesh> m_lamp;
        std::shared_ptr<SkyboxMesh> m_skybox;
        std::shared_ptr<QuadMesh> m_quad;
        std::shared_ptr<QuadMesh> m_post_quad;
        std::shared_ptr<LightingQuadMesh> m_lighting_quad;
        std::shared_ptr<VolumeMesh> m_volume_mesh;
        std::shared_ptr<DefaultShader> m_default_shader;
        std::shared_ptr<QuadShader> m_quad_shader;
        std::shared_ptr<PostProcessingShader> m_post_shader;
        std::shared_ptr<GBufferShader> m_gbuffer_shader;
        std::shared_ptr<ShaderSkyBox> m_gbuffer_skybox_shader;
        std::shared_ptr<LightingShader> m_lighting_shader;
        std::shared_ptr<LightingSkyboxShader> m_lighting_skybox_shader;
        GLuint m_tex_white;
        GLuint m_tex_grey;
        GLuint m_tex_black;
        GLuint m_tex_blue;
        GLuint m_tex_grass;
        GLuint m_tex_mars;
        GLuint m_tex_skybox;
        // Default lighting properties
        GLuint m_tex_lighting;
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

          m_logger.logDebug("Compiling lighting skybox shader");
          m_lighting_skybox_shader = std::make_shared<LightingSkyboxShader>();

          m_logger.logDebug("Compiling post processing shader");
          m_post_shader = std::make_shared<PostProcessingShader>();

          m_logger.logDebug("Building cube mesh");
          m_cube = std::make_shared<GenericMesh>("Models/cube.obj", getGBufferShader());

          m_logger.logDebug("Building lamp mesh");
          m_lamp = std::make_shared<GenericMesh>("Models/lamp.obj", getGBufferShader(), 1.0/184);

          m_logger.logDebug("Building skybox mesh");
          m_skybox = std::make_shared<SkyboxMesh>("Models/cube.obj", getGBufferShaderSkybox());

          m_logger.logDebug("Building sphere mesh");
          m_sphere = std::make_shared<GenericMesh>("Models/sphere.obj", getGBufferShader());

          m_logger.logDebug("Building quad mesh");
          m_quad = std::make_shared<QuadMesh>(getQuadShader());

          m_logger.logDebug("Building post quad mesh");
          m_post_quad = std::make_shared<QuadMesh>(getPostProcessingShader());

          m_logger.logDebug("Building lighting quad mesh");
          m_lighting_quad = std::make_shared<LightingQuadMesh>(getLightingSkyboxShader());

          m_logger.logDebug("Building volume quad mesh");
          m_volume_mesh = std::make_shared<VolumeMesh>(getLightingShader());

          m_logger.logDebug("Generating textures");
          m_tex_white = createTextureColor(1, 1, 1);
          m_tex_grey = createTextureColor(0.7, 0.7, 0.7);
          m_tex_black = createTextureColor(0, 0, 0);
          m_tex_blue = createTextureColor(0.2f, 0.2f, 1.0f);
          m_tex_grass = loadTexture("Textures/grass.jpg");
          m_tex_mars = loadTexture("Textures/mars.jpg");
          m_tex_lighting = createTextureColor(0.15, 0.55, 0.3);

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
        inline const GenericMesh* getLampMesh() const { return m_lamp.get(); }
        inline const SkyboxMesh* getSkyboxMesh() const { return m_skybox.get(); }
        inline const QuadMesh* getQuadMesh() const { return m_quad.get(); }
        inline const QuadMesh* getPostQuadMesh() const { return m_post_quad.get(); }
        inline const LightingQuadMesh* getLightingQuadMesh() const { return m_lighting_quad.get(); }
        inline const VolumeMesh* getVolumeMesh() const { return m_volume_mesh.get(); }
        inline const DefaultShader* getDefaultShader() const { return m_default_shader.get(); }
        inline const QuadShader* getQuadShader() const { return m_quad_shader.get(); }
        inline const PostProcessingShader* getPostProcessingShader() const { return m_post_shader.get(); }
        inline const GBufferShader* getGBufferShader() const { return m_gbuffer_shader.get(); }
        inline const ShaderSkyBox* getGBufferShaderSkybox() const { return m_gbuffer_skybox_shader.get(); }
        inline const LightingShader* getLightingShader() const { return m_lighting_shader.get(); }
        inline const LightingSkyboxShader* getLightingSkyboxShader() const { return m_lighting_skybox_shader.get(); }
        inline const GLuint getWhiteTexture() const { return m_tex_white; }
        inline const GLuint getGrayTexture() const { return m_tex_grey; }
        inline const GLuint getBlackTexture() const { return m_tex_black; }
        inline const GLuint getBlueTexture() const { return m_tex_blue; }
        inline const GLuint getGrassTexture() const { return m_tex_grass; }
        inline const GLuint getMarsTexture() const { return m_tex_mars; }
        inline const GLuint getSkyBoxTexture() const { return m_tex_skybox; }
        inline const GLuint getLightingTexture() const { return m_tex_lighting; }

};
