link_libraries(Coelengine)

function(CREATE_EXAMPLE FOLDER SUBFOLDER SOURCE_FILE)
    add_executable(
        "Examples_${FOLDER}_${SUBFOLDER}_${SOURCE_FILE}"
        "${FOLDER}/${SUBFOLDER}/${SOURCE_FILE}.cpp"
    )
endfunction()

# GETTING STARTED EXAMPLES
CREATE_EXAMPLE(1_GettingStarted 1_Window 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 1_Window 2_Clear)

CREATE_EXAMPLE(1_GettingStarted 2_Triangle 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 2_Triangle 2_Indexed)

CREATE_EXAMPLE(1_GettingStarted 3_Shader 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 3_Shader 2_Uniform)
CREATE_EXAMPLE(1_GettingStarted 3_Shader 3_Varying)

CREATE_EXAMPLE(1_GettingStarted 4_Texture 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 4_Texture 2_Multiple)

CREATE_EXAMPLE(1_GettingStarted 5_Model 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 5_Model 2_Textured)

CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 1_Invert)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 2_Sharpen)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 3_SimpleBlur)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 4_GaussianBlur)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 5_Deferred)

CREATE_EXAMPLE(1_GettingStarted 7_Text 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 7_Text 2_Input)
CREATE_EXAMPLE(1_GettingStarted 7_Text 3_ImGui)

# 3D LIGHTING EXAMPLES
CREATE_EXAMPLE(2_Lighting 1_Phong 1_Diffuse)
CREATE_EXAMPLE(2_Lighting 1_Phong 2_Specular)
CREATE_EXAMPLE(2_Lighting 1_Phong 3_Combined)

CREATE_EXAMPLE(2_Lighting 2_Fog 1_Simple)
CREATE_EXAMPLE(2_Lighting 3_Shadow 1_Simple)
CREATE_EXAMPLE(2_Lighting 4_AmbientOcclusion 1_SSAO)

CREATE_EXAMPLE(2_Lighting 5_PBR 1_BRDF)
CREATE_EXAMPLE(2_Lighting 5_PBR 2_ImageBased)

# REAL WORLD EXAMPLES
CREATE_EXAMPLE(3_RealWorld 1_UserInterface Main)
