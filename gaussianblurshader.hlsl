// Input and output resources
Texture2D<float4> sourceImage : register(t1);
RWTexture2D<float4> blurImage : register(u0);

// Group size
#define size_x 32
#define size_y 32

// Define the filter
static const float filter[7][7] = {
	0.000904706, 0.003157733, 0.00668492, 0.008583607, 0.00668492, 0.003157733, 0.000904706,
	0.003157733, 0.01102157, 0.023332663, 0.029959733, 0.023332663, 0.01102157, 0.003157733,
	0.00668492, 0.023332663, 0.049395249, 0.063424755, 0.049395249, 0.023332663, 0.00668492,
	0.008583607, 0.029959733, 0.063424755, 0.081438997, 0.063424755, 0.029959733, 0.008583607,
	0.00668492, 0.023332663, 0.049395249, 0.063424755, 0.049395249, 0.023332663, 0.00668492,
	0.003157733, 0.01102157, 0.023332663, 0.029959733, 0.023332663, 0.01102157, 0.003157733,
	0.000904706, 0.003157733, 0.00668492, 0.008583607, 0.00668492, 0.003157733, 0.000904706
};

// Declare one thread for each pixel in the group
[numthreads(size_x, size_y, 1)]

void cs_main( uint3 DTid : SV_DispatchThreadID )
{
	// Offset texture location to first sample location
	int3 tex_location = DTid - int3(3, 3, 0);

	// Initialize output to 0 and loop through sample locations adding up
	float4 color = float4(0.0, 0.0, 0.0, 0.0);

	for (int x = 0; x < 7; ++x) 
		for (int y = 0; y < 7; ++y) 
			color += sourceImage.Load(tex_location + int3(x, y, 0)) * filter[x][y];

	// Write to output
	blurImage[DTid.xy] = color;
}