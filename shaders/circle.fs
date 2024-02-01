#version 330

// Input vertex attr. (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output frag. color 
out vec4 finalColor;

void main() {
  finalColor = vec4(0,1,0,1);
}
