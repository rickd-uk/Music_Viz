
#version 330

// Input vertex attr. (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output frag. color 
out vec4 finalColor;

void main() {
  float radius = 0.05;
  vec2 p = fragTexCoord - vec2(0.5);
  if (length(p)  <= 0.5) {
    float s = length(p) - radius;
    if (s <= 0) {
      finalColor = fragColor * 1.25;
    } else {
      float t = 1 - s / (0.5 - radius);
      finalColor = vec4(fragColor.xyz,t*t*t*t);
    }
  } else {
    finalColor = vec4(0);
  }
}
