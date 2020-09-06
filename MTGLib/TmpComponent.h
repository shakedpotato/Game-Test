#pragma once
#include "CommonWinApp.h"
#include "ComponentBase.h"

#include "Timer.h"

class TmpComponent
	:
	public ComponentBase
{
public:
	TmpComponent()
		:
		ComponentBase("TmpComponent")
	{

	}
	~TmpComponent()
	{
		
	}

public:
	void Tmp()
	{
		MyOutputDebugString("TMPTMPTMP\n");
	}
private:
	void OnAwake() override
	{
		MyOutputDebugString("Tmp OnAwake\n");
	}

	void OnStart() override
	{
		MyOutputDebugString("Tmp OnStart\n");
	}
};