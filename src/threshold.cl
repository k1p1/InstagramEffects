//
//  threshold.cl
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

__kernel void threshold(__read_only  image2d_t srcImg,
                         __write_only image2d_t dstImg)
{
    const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | //Natural coordinates
    CLK_ADDRESS_CLAMP_TO_EDGE | //Clamp to zeros
    CLK_FILTER_NEAREST;
    int2 coord = (int2)(get_global_id(0), get_global_id(1));
    uint4 bgra = read_imageui(srcImg, smp, coord);
    
    if (bgra.x + bgra.y + bgra.z < 128*3)
    {
        bgra.x = 0;
        bgra.y = 0;
        bgra.z = 0;
    }
    else
    {
        bgra.x = 256;
        bgra.y = 256;
        bgra.z = 256;
    }
    
    write_imageui(dstImg, coord, bgra);
}