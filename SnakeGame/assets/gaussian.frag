//declare uniforms
uniform sampler2D texture;
uniform float resolution;
uniform vec2 dir;

uniform float blurAmount;
uniform float blurScale;
uniform float blurStrength;

float Gaussian (float x, float deviation)
{
    return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));  
}

void main() {
	//this will be our RGBA sum
	vec4 sum = vec4(0.0);
	
	//our original texcoord for this fragment
	vec2 tc = gl_TexCoord[0].xy;
    
	//the direction of our blur
	//(1.0, 0.0) -> x-axis blur
	//(0.0, 1.0) -> y-axis blur
	float hstep = dir.x;
	float vstep = dir.y;

	float deviation = blurAmount * 0.5 * 0.35;
    deviation *= deviation;
	float str = 1.0 - blurStrength;

    for (int i = 0; i < 100; ++i)
    {
    	if ( i >= blurAmount )
            break;

        float offset = float(i) - blurAmount * 0.5;
        float comp = offset * 1.0/resolution * blurScale;
        sum += texture2D(texture, tc + vec2(comp * hstep, comp * vstep)) 
        * Gaussian(offset * str, deviation);
    }

	gl_FragColor = clamp(sum, 0.0, 1.0);
    //gl_FragColor.w = 1.0;
}