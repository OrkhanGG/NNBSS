#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

class CNNBSSAboutPage
{
public:
	CNNBSSAboutPage() {}
	~CNNBSSAboutPage() {};

    void _RequestCreate();

private:
    void DecorateSplashScreen(wxBitmap& bmp);
};

#endif // ! ABOUTPAGE_H