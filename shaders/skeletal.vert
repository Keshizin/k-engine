#version 420 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec4 vColor;
layout (location=4) in vec4 vJoint;
layout (location=5) in vec4 vWeight;

layout (location=0) out vec4 outColor;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

uniform int hasSkinAnimation = 0;

uniform mat4 jointMatrices[89];
uniform mat4 globalJointMatrices[89];

void main() {
    mat4 skinMatrix = mat4(1.0);

    if(hasSkinAnimation == 1) 
        skinMatrix =
            vWeight.x * globalJointMatrices[int(vJoint.x)] * jointMatrices[int(vJoint.x)] + // joint 1
            vWeight.y * globalJointMatrices[int(vJoint.y)] * jointMatrices[int(vJoint.y)] + // joint 2
            vWeight.z * globalJointMatrices[int(vJoint.z)] * jointMatrices[int(vJoint.z)] + // joint 3
            vWeight.w * globalJointMatrices[int(vJoint.w)] * jointMatrices[int(vJoint.w)]; // joint 4

    vec4 skinnedPosition = skinMatrix * vec4(vPosition, 1.0);
	
    gl_Position = projectionTransform * viewTransform * modelTransform * skinnedPosition;
	outColor = vColor;
    outColor = skinnedPosition;
}