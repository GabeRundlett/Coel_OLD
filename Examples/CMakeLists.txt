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
CREATE_EXAMPLE(1_GettingStarted 1_Window 3_Events)

CREATE_EXAMPLE(1_GettingStarted 2_Triangle 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 2_Triangle 2_Indexed)

CREATE_EXAMPLE(1_GettingStarted 3_Shader 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 3_Shader 2_Uniform)
CREATE_EXAMPLE(1_GettingStarted 3_Shader 3_Varying)

CREATE_EXAMPLE(1_GettingStarted 4_Texture 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 4_Texture 2_Filtering)
CREATE_EXAMPLE(1_GettingStarted 4_Texture 3_Wrapping)
CREATE_EXAMPLE(1_GettingStarted 4_Texture 4_Multiple)

CREATE_EXAMPLE(1_GettingStarted 5_Model 1_Simple)
CREATE_EXAMPLE(1_GettingStarted 5_Model 2_Textured)

CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 1_Invert)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 2_Sharpen)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 3_SimpleBlur)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 4_GaussianBlur)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 5_Deferred)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 6_SuperSample)
CREATE_EXAMPLE(1_GettingStarted 6_PostProcess 7_SSAO)

# CREATE_EXAMPLE(1_GettingStarted 7_Text 1_Simple)
# CREATE_EXAMPLE(1_GettingStarted 7_Text 2_Input)

# CREATE_EXAMPLE(1_GettingStarted 8_Renderers 2_Batch2d)
CREATE_EXAMPLE(1_GettingStarted 8_Renderers 3_CustomBatch)
# CREATE_EXAMPLE(1_GettingStarted 8_Renderers 4_Sprites)

CREATE_EXAMPLE(1_GettingStarted 9_ImGui 1_Demo)
CREATE_EXAMPLE(1_GettingStarted 9_ImGui 2_Dockspace)

# 3D LIGHTING EXAMPLES
CREATE_EXAMPLE(2_Lighting 1_Phong 1_Diffuse)
CREATE_EXAMPLE(2_Lighting 1_Phong 2_Specular)
CREATE_EXAMPLE(2_Lighting 1_Phong 3_Combined)

CREATE_EXAMPLE(2_Lighting 2_Fog 1_Simple)
CREATE_EXAMPLE(2_Lighting 3_Shadow 1_Simple)
CREATE_EXAMPLE(2_Lighting 3_Shadow 2_Vxgi)

CREATE_EXAMPLE(2_Lighting 5_PBR 1_BRDF)
CREATE_EXAMPLE(2_Lighting 5_PBR 2_ImageBased)

# REAL WORLD EXAMPLES
CREATE_EXAMPLE(3_RealWorld 1_TerrainGen 1_PlaneGen)
CREATE_EXAMPLE(3_RealWorld 1_TerrainGen 2_HeightMap)

# CREATE_EXAMPLE(3_RealWorld 2_MarchingCubes 1_PointGen)
CREATE_EXAMPLE(3_RealWorld 3_Platformer Main)
# CREATE_EXAMPLE(3_RealWorld 4_Splines 1_Bezier)
# CREATE_EXAMPLE(3_RealWorld 4_Splines 2_CatmullRom)

CREATE_EXAMPLE(3_RealWorld 5_Minecraft 1_Renderer)
CREATE_EXAMPLE(3_RealWorld 5_Minecraft 2_Player)

CREATE_EXAMPLE(3_RealWorld 6_SpriteAnimation 1_SpriteAnimation)
CREATE_EXAMPLE(3_RealWorld 7_CellularAutomata 1_gameoflife)
CREATE_EXAMPLE(3_RealWorld 7_CellularAutomata 2_CellEditor)
