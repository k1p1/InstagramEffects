//
//  helloWorld.cl
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

__kernel void helloWorld(
	__global float* input,                                                               
	__global float* output,                                                              
	const unsigned int count)                                                            
{
	int id = get_global_id(0);
    
    if(id < count)
	{
		output[id] = input[id] * input[id];
	}
}
