///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef LINEFLOWGLOBAL_H
#define LINEFLOWGLOBAL_H

class wxMutex;

class CNNBSSLineFlow
{
public:
	CNNBSSLineFlow();
	~CNNBSSLineFlow();

	void OnStart();
};

#endif // ! LINEFLOWGLOBAL_H