class ResourceRepo
    {
    private:
        Logger m_logger = Logger("ResourceRepo");
        std::shared_ptr<CubeMesh> m_cube;
        std::shared_ptr<DefaultShader> m_default_shader;
    public:
        ResourceRepo(){
          m_logger.logDebug("Compiling default shader");
          m_default_shader = std::make_shared<DefaultShader>();

          m_logger.logDebug("Building cube mesh");
          m_cube = std::make_shared<CubeMesh>(getDefaultShader());
        };
        inline const CubeMesh* getCubeMesh() const { return m_cube.get(); }
        inline const DefaultShader* getDefaultShader() const { return m_default_shader.get(); }

};
