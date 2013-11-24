#ifndef HSV_H
#define HSV_H

template<class T> T MIN(T a, T b, T c)
{
	if(a <= b && a <= c)
		return a;
	if(b <= a && b <= c)
		return b;
	if(c <= a && c <= b)
		return c;
	return -1;
}

template<class T> T MAX(T a, T b, T c)
{
	if(a >= b && a >= c)
		return a;
	if(b >= a && b >= c)
		return b;
	if(c >= a && c >= b)
		return c;
	return -1;
}

void RGBtoHSV( int cr, int cg, int cb, int *h, int *s, int *v )
{
	float r = (float)cr / 255;
	float g = (float)cg / 255;
	float b = (float)cb / 255;
	float min, max, delta;

	min = MIN( r, g, b );
	max = MAX( r, g, b );
	*v = (int)(max * 255);				// v

	delta = max - min;

	if( max != 0 )
		*s = (int)(delta * 255 / max);		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = 0;
		return;
	}

	float hh;
	if( r == max )
		hh = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		hh = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		hh = 4 + ( r - g ) / delta;	// between magenta & cyan

	hh *= 60;				// degrees
	if( hh < 0 )
		hh += 360;
	*h = (int)(hh * 255 / 360); // scale
}

void HSVtoRGB(int *r, int *g, int *b, int ch, int cs, int cv)
{
	float h = (float)ch * 360 / 255;
	float s = (float)cs / 255;
	float v = (float)cv / 255;

	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = (int)(v * 255);
		return;
	}

	h /= 60;			// sector 0 to 5
	i = static_cast<int>(h);
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			*r = (int)(v * 255);
			*g = (int)(t * 255);
			*b = (int)(p * 255);
			break;
		case 1:
			*r = (int)(q * 255);
			*g = (int)(v * 255);
			*b = (int)(p * 255);
			break;
		case 2:
			*r = (int)(p * 255);
			*g = (int)(v * 255);
			*b = (int)(t * 255);
			break;
		case 3:
			*r = (int)(p * 255);
			*g = (int)(q * 255);
			*b = (int)(v * 255);
			break;
		case 4:
			*r = (int)(t * 255);
			*g = (int)(p * 255);
			*b = (int)(v * 255);
			break;
		default:		// case 5:
			*r = (int)(v * 255);
			*g = (int)(p * 255);
			*b = (int)(q * 255);
			break;
	}
}

#endif