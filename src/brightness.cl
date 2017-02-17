//
//  brightness.cl
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

__kernel void brightness(__read_only  image2d_t srcImg,
                        __write_only image2d_t dstImg)
{
    const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | //Natural coordinates
    CLK_ADDRESS_CLAMP_TO_EDGE | //Clamp to zeros
    CLK_FILTER_NEAREST;
    int2 coord = (int2)(get_global_id(0), get_global_id(1));
    uint4 bgra = read_imageui(srcImg, smp, coord);
    
    bgra.x += 100;
    bgra.y += 100;
    bgra.z += 100;
    bgra.w += 100;
    
    write_imageui(dstImg, coord, bgra);
}
