BUILD_DIR=$1
GLSLC=/Users/loveucifer/VulkanSDK/1.4.335.1/macOS/bin/glslc
$GLSLC ../shaders/SuzumeShader.vert -o $BUILD_DIR/SuzumeShader.vert.spv
$GLSLC ../shaders/SuzumeShader.frag -o $BUILD_DIR/SuzumeShader.frag.spv
cp $BUILD_DIR/SuzumeShader.vert.spv ../shaders/
cp $BUILD_DIR/SuzumeShader.frag.spv ../shaders/
