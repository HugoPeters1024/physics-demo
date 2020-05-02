class IQuadShader {
public:
    virtual void use(GLuint texture) const = 0;

    static int SH_IN_VPOS;
};

int IQuadShader::SH_IN_VPOS = 0;
