#version 330 core
uniform vec3 playerLightPosition;
uniform vec3 upperLightPosition;
uniform vec3 viewPosition;

out vec4 color;

in vec3 fragmentColor;

in vec3 normalVector;
in vec3 fragmentPosition;

void main()
{
    /*vec3 bubbleColor = fragmentColor;
    vec3 upperLightColor = vec3(1.0, 1.0, 1.0);
    vec3 playerLightColor = vec3(1.0, 1.0, 1.0);
    vec3 waterAbsorb = vec3(4.0, 2.0, 0.1);

    //float upperAmbientStrength = 0.1;
    float playerAmbientStrength = 0.1;
    //vec3 upperAmbient = upperAmbientStrength * upperLightColor;
    vec3 playerAmbient = playerAmbientStrength * playerLightColor;

    vec3 norm = normalize(normalVector);
    vec3 playerLightDir = normalize(playerLightPosition - fragmentPosition);
    //vec3 upperLightDir = normalize(upperLightPosition - fragmentPosition);

    float diffPl = max(dot(norm, playerLightDir), 0.0);
    //float diffUp = max(dot(norm, upperLightDir), 0.0);
    float strDistDependendPlayer = min((1/ (distance(playerLightPosition, fragmentPosition)/10))/ 20, 1.0);
    //float strDistDependendUpper = min((1/ (distance(upperLightPosition, fragmentPosition)/10))/5, 1.0);
    //vec3 diffuse = diffPl * playerLightColor * strDistDependendPlayer + diffUp * upperLightColor * strDistDependendUpper;
    //vec3 diffuse = diffPl * playerLightColor  + diffUp * upperLightColor;
    vec3 diffuse = diffPl * playerLightColor * strDistDependendPlayer;//  + diffUp * upperLightColor;
    float playerSpecularStrength = 0.5;
    //float upperSpecularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - fragmentPosition);
    vec3 playerReflectDir = reflect(-playerLightDir, norm);
    //vec3 uppperReflectDir = reflect(-upperLightDir, norm);
    int shinesses = 32;
    float specPl = pow(max(dot(viewDir, playerReflectDir), 0.0), shinesses);
   // float specUp = pow(max(dot(viewDir, uppperReflectDir), 0.0), shinesses);
    //vec3 specular = playerSpecularStrength * specPl * playerLightColor + upperSpecularStrength * specUp * upperLightColor;
    vec3 specular = playerSpecularStrength * specPl * playerLightColor;// + upperSpecularStrength * specUp * upperLightColor;
    //vec3 result = (playerAmbient + upperAmbient + diffuse + specular) * bubbleColor;
    vec3 result = (playerAmbient + diffuse + specular) * bubbleColor;
    //float strDistDependendView = min(((distance(viewPosition, fragmentPosition)/10))/15, 1.0);
    //vec3 absorb = exp(-waterAbsorb * strDistDependendView);
    //result *= absorb;

    color = vec4(result, 0.8);*/

    vec3 bubbleColor = fragmentColor;
    vec3 upperLightColor = vec3(1.0, 1.0, 1.0);
    vec3 playerLightColor = vec3(1.0, 1.0, 1.0);
    vec3 waterAbsorb = vec3(4.0, 2.0, 0.1);

    float upperAmbientStrength = 0.1;
    float playerAmbientStrength = 0.1;
    vec3 upperAmbient = upperAmbientStrength * upperLightColor;
    vec3 playerAmbient = playerAmbientStrength * playerLightColor;

    vec3 norm = normalize(normalVector);
    vec3 playerLightDir = normalize(playerLightPosition - fragmentPosition);
    vec3 upperLightDir = normalize(upperLightPosition - fragmentPosition);

    float diffPl = max(dot(norm, playerLightDir), 0.0);
    float diffUp = max(dot(norm, upperLightDir), 0.0);
    float strDistDependendPlayer = min((1/ (distance(playerLightPosition, fragmentPosition)/10))/ 20, 1.0);
    //float strDistDependendUpper = min((1/ (distance(upperLightPosition, fragmentPosition)/10))/5, 1.0);
    vec3 diffuse = diffPl * playerLightColor * strDistDependendPlayer + diffUp * upperLightColor;
    //vec3 diffuse = diffPl * playerLightColor  + diffUp * upperLightColor;
    //vec3 diffuse = diffPl * playerLightColor * strDistDependendPlayer;//  + diffUp * upperLightColor;
    float playerSpecularStrength = 0.5;
    float upperSpecularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - fragmentPosition);
    vec3 playerReflectDir = reflect(-playerLightDir, norm);
    vec3 uppperReflectDir = reflect(-upperLightDir, norm);
    int shinesses = 32;
    float specPl = pow(max(dot(viewDir, playerReflectDir), 0.0), shinesses);
    float specUp = pow(max(dot(viewDir, uppperReflectDir), 0.0), shinesses);
    vec3 specular = playerSpecularStrength * specPl * playerLightColor + upperSpecularStrength * specUp * upperLightColor;
    //vec3 specular = playerSpecularStrength * specPl * playerLightColor;// + upperSpecularStrength * specUp * upperLightColor;
    vec3 result = (playerAmbient + upperAmbient + diffuse + specular) * bubbleColor;
    //vec3 result = (playerAmbient + diffuse + specular) * bubbleColor;
    float strDistDependendView = min(((distance(viewPosition, fragmentPosition)/10))/60, 1.0);
    vec3 absorb = exp(-waterAbsorb * strDistDependendView);
    result *= absorb;

    color = vec4(result, 1.0);
}