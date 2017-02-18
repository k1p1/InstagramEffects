__kernel void convolution(__read_only image2d_t srcImg,
                          __write_only image2d_t dstImg,
                          __global float * filterValues,
                          __global int * filterSize)
{
    const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | //Natural coordinates
    CLK_ADDRESS_CLAMP | //Clamp to zeros
    CLK_FILTER_NEAREST; //Don't interpolate
    
    int x = get_global_id(0);
    int y = get_global_id(1);
    
    float4 convPix = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 temp;
    uint4 pix;
    int2 coords;
    
    for (int local_x = 0; local_x < *filterSize; local_x++)
    {
        for (int local_y = 0; local_y < *filterSize; local_y++)
        {
            coords.x = x + local_x;
            coords.y = y + local_y;
            
            pix = read_imageui(srcImg, smp, coords);
            
            temp = (float4)((float)pix.x, (float)pix.y, (float)pix.z, (float)pix.w);
            convPix += temp * filterValues[local_x + (*filterSize) * local_y];
        }
    }
    coords.x = x;
    coords.y = y;
 
    pix = (uint4)((uint)convPix.x, (uint)convPix.y, (uint)convPix.z, (uint)convPix.w);
    
    write_imageui(dstImg, coords, pix);
}