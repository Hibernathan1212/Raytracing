#version 450
in vec4 gl_FragCoord;

struct Material
{
    vec3 Albedo;
    float Roughness;
    vec3 Emission;
    //float Metallic;
};

//struct Sphere
//{
//    vec3 Position;
//    float Radius;
//
//    Material material;
//};

struct Triangle
{
    vec3 A;
    vec3 B;
    vec3 C;
};

struct Vertex
{
    float A;
    float B;
    float C;
};

struct Object
{
    vec3 Vertices[128];
    ivec3 Indices[126];
    Material material;
    uint numberOfIndices;
};

struct CameraData
{
    vec3 CameraOrigin;
    float ViewportWidth;
    float ViewportHeight;
    mat4 Projection;
    mat4 View;
    mat4 InverseProjection;
    mat4 InverseView;
};

layout(binding = 0) uniform UniformData {
    CameraData cameraData;
    Object Objects[10];
    uint numberOfObjects;
} u_Data;

layout(location = 0) out vec4 outColor;

struct Ray
{
    vec3 Origin;
    vec3 Direction;
};

struct HitPayload
{
    float HitDistance;
    vec3 WorldPosition;
    vec3 WorldNormal;

    int ObjectIndex;
};

vec3 CalculateRayDirection(float x, float y);

vec4 PerPixel(float x, float y);

HitPayload TraceRay(Ray ray);

HitPayload ClosestHit(Ray ray, float hitDistance, int objectIndex);

HitPayload Miss(Ray ray);

//vec4 TraceRay(Ray ray);

float rand(vec3 c0)
{
    return fract(sin(dot(c0, vec3(12.9898, 357, 23.829))) * 43758.5453);
    
    //vec2 r = {0.0f, 0.0f};
    //vec3  uu = normalize(cross(c0, abs(c0.y) > .5 ? vec3(1.,0.,0.) : vec3(0.,1.,0.)));
    //vec3  vv = cross(uu, c0);
    //float ra = sqrt(r.y);
    //float rx = ra*cos(6.28318530718*r.x);
    //float ry = ra*sin(6.28318530718*r.x);
    //float rz = sqrt(1.-r.y);
    //vec3  rr = vec3(rx*uu + ry*vv + rz*c0);
    //return normalize(rr);
}

void main()
{
    vec4 color = PerPixel(gl_FragCoord.x, gl_FragCoord.y);
        
    color = clamp(sqrt(color), vec4(0.0f), vec4(1.0f));
    outColor = color;
}

vec3 CalculateRayDirection(float x, float y)
{
    vec2 coord = { (x) / u_Data.cameraData.ViewportWidth, (y) / u_Data.cameraData.ViewportHeight };
    coord = coord * 2.0f - 1.0f; // -1 -> 1

    vec4 target = u_Data.cameraData.InverseProjection * vec4(coord.x, coord.y, 1, 1);
    vec3 rayDirection = vec3(u_Data.cameraData.InverseView * vec4(normalize(vec3(target) / target.w), 0)); // World space
    return rayDirection;
}

vec4 PerPixel(float x, float y)
{
    Ray ray;
    
    vec3 color = vec3(0.0f, 0.0f, 0.0f);

    int paths = 10;
    int bounces = 4;

    for (int i = 0; i < paths; i++)
    {
        ray.Direction = CalculateRayDirection(gl_FragCoord.x, gl_FragCoord.y);
        ray.Origin = u_Data.cameraData.CameraOrigin;
    
        vec3 light = vec3(0.0f, 0.0f, 0.0f);
    
        vec3 contribution = vec3(1.0f, 1.0f, 1.0f);

        for (int j = 0; j < bounces; j++)
        {
            HitPayload payload = TraceRay(ray);
            if (payload.HitDistance < 0.0f)
            {
                vec3 skyColor = vec3(0.3f, 0.35f, 0.45f);
                //vec3 skyColor = vec3(0.0f);
                light += skyColor * contribution;
                break;
            }
            
            //Sphere sphere = u_Data.objectData.Spheres[payload.ObjectIndex];
            Material material = u_Data.Objects[payload.ObjectIndex].material;
            
            //light += material.Albedo * multiplier;
            light += material.Emission * contribution;
            contribution *= material.Albedo;
    
            ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
            //ray.Direction = reflect(ray.Direction, payload.WorldNormal);// + material.Roughness * vec3(rand(payload.WorldPosition*(i+2)/2)));
            if (material.Roughness == 0.0f)
            {
                ray.Direction = reflect(ray.Direction, payload.WorldNormal);
                continue;
            }
            float rand1 = rand(payload.WorldPosition*(i+1));
            float rand2 = rand(payload.WorldPosition*2*(i+1));
            float rand3 = rand(payload.WorldPosition*3*(i+1));
            
            vec3 diffuseDirection = normalize(0.5f * payload.WorldNormal + vec3(rand1 - rand2, rand2 - rand3, rand3 - rand1));
            vec3 specularDirection = reflect(ray.Direction, payload.WorldNormal);// + material.Roughness * vec3(rand(payload.WorldPosition*(i+2)/2)));
            ray.Direction = mix(specularDirection, diffuseDirection, material.Roughness);
        }
    
        color += light/paths;
    }
    return vec4(color, 1.0f);
}

HitPayload TraceRay(Ray ray)
{
    int closestObject = -1;
    float hitDistance = 1.0f / 0.0f;
    vec3 WorldNormal;
    
    for (int i = 0; i < u_Data.numberOfObjects; i++)
    {
        for (int j = 0; j < u_Data.Objects[i].numberOfIndices; j++)
        {
            Triangle triangle;
            
            triangle.A = u_Data.Objects[i].Vertices[u_Data.Objects[i].Indices[j].x];
            triangle.B = u_Data.Objects[i].Vertices[u_Data.Objects[i].Indices[j].y];
            triangle.C = u_Data.Objects[i].Vertices[u_Data.Objects[i].Indices[j].z];
            
            if (triangle.A == triangle.B || triangle.B == triangle.C)
                continue;
            
            vec3 edgeAB = triangle.B - triangle.A;
            vec3 edgeAC = triangle.C - triangle.A;
            vec3 normalVector = cross(edgeAB, edgeAC);
            
            float det = -dot(ray.Direction, normalVector);
            
            if (det == 0)
                continue;
            
            float invDet = 1 / det;
            
            vec3 ao = ray.Origin - triangle.A;
            
            float dst = dot(ao, normalVector) * invDet;
            if (dst < 0)
                continue;
            
            vec3 dao = cross(ao, ray.Direction);
            
            float u = dot(edgeAC, dao) * invDet;
            if (u < 0)
                continue;
            
            float v = -dot(edgeAB, dao) * invDet;
            if (v < 0)
                continue;
            
            //float w = 1 - u - v;
            if (u + v > 1)
                continue;
                    
            if (dst < hitDistance)
            {
                hitDistance = dst;
                closestObject = i;
                WorldNormal = normalize(normalVector * det);
            }
        }
    }
    
    if (closestObject < 0)
        return Miss(ray);
    
    HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = closestObject;
    payload.WorldPosition = ray.Origin + ray.Direction * hitDistance;
    payload.WorldNormal = WorldNormal;
    
    return payload;
}

//HitPayload TraceRay(Ray ray)
//{
//    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
//    // where
//    // a = ray origin
//    // b = ray direction
//    // r = radius
//    // t = hit distance
//
//    int closestSphere = -1;
//    float hitDistance = 1.0f / 0.0;
//    for (int i = 0; i < u_Data.objectData.Objects; i++)
//    {
//        const Sphere sphere = u_Data.objectData.Spheres[i];
//        if (sphere.Radius == 0.0f)
//            continue;
//
//        vec3 origin = ray.Origin - sphere.Position;
//
//        float a = dot(ray.Direction, ray.Direction);
//        float b = 2.0f * dot(origin, ray.Direction);
//        float c = dot(origin, origin) - sphere.Radius * sphere.Radius;
//
//        float discriminant = b * b - 4.0f * a * c;
//
//        if (discriminant < 0.0f)
//            continue;
//
//        float closestT = (-b - sqrt(discriminant)) / (2.0f * a);
//        if (closestT > 0.0f && closestT < hitDistance)
//        {
//            hitDistance = closestT;
//            closestSphere = i;
//        }
//    }
//
//    if (closestSphere < 0)
//        return Miss(ray);
//
//    return ClosestHit(ray, hitDistance, closestSphere);
//}

//HitPayload ClosestHit(Ray ray, float hitDistance, int objectIndex)
//{
//    HitPayload payload;
//    payload.HitDistance = hitDistance;
//    payload.ObjectIndex = objectIndex;
//
//    const Sphere closestSphere = u_Data.objectData.Spheres[objectIndex];
//
//    vec3 origin = ray.Origin - closestSphere.Position;
//
//    payload.WorldPosition = origin + ray.Direction * hitDistance;
//    payload.WorldNormal = normalize(payload.WorldPosition);
//    payload.WorldPosition += closestSphere.Position;
//
//    return payload;
//}

HitPayload Miss(Ray ray)
{
    HitPayload payload;
    payload.HitDistance = -1.0f;
    return payload;
}
