//Use includes = error!
struct Vertex
{
    float4 position: POSITION;
    float4 color: COLOR;
};

Vertex VertexStart(float4 pos: POSITION, float4 color: COLOR): Vertex
{
    Vertex vertex;
    vertex.position = pos;
    vertex.color = color;

    return vertex;
}

float4 FragStart(Vertex vertex: Vertex): SV_Target
{
    return vertex.color;
}