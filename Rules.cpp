#include "TextStruct.h"

Rules::Rules()
{
	emphs		= new RuleStack(10,0);
	pitchs		= new RuleStack(10,2);
	rates		= new RuleStack(10,3);
	volumes		= new RuleStack(10,4);
	engines		= new RuleStack(10,5);
	sayass		= new RuleStack(10,8);
	langs		= new RuleStack(10,9);
	speakers	= new RuleStack(10,10);
	audios		= new RuleStack(10,11);
	markers		= new RuleStack(10,12);
	divs		= new RuleStack(10,13);

	defaults();
}

Rules::~Rules()
{
	delete emphs;
	delete pitchs;
	delete rates;
	delete volumes;
	delete engines;
	delete sayass;
	delete langs;
	delete speakers;
	delete audios;
	delete markers;
	delete divs;
}

void Rules::defaults()
{
	int data;
	this->sableOn = FALSE;

	emphs->MakeEmpty();		// 0
	pitchs->MakeEmpty();	// 1
	rates->MakeEmpty();		// 2
	volumes->MakeEmpty();	// 3
	engines->MakeEmpty();	// 4
	sayass->MakeEmpty();	// 5
	langs->MakeEmpty();		// 6
	speakers->MakeEmpty();	// 7

	// Default: Moderate
	emphs->Push(EL_MODERATE);
	// Default: Base=Default & Middle=Default & Range=Default
	data = 100*PB_DEFAULT + 10*PM_DEFAULT + PR_DEFAULT;
	pitchs->Push(data);
	// Default: Medium
	rates->Push(RS_MEDIUM);
	// Default: Medium
	volumes->Push(VL_MEDIUM);
	// Default: Phonemes
	engines->Push(EI_PH);
	// Default: 
	data = SM_LITERAL * 100;
	sayass->Push(data);
	// Default: Turkish
	langs->Push(LI_TR);
	// Default : Gender=Male & Age=Middle
	data = 10*SG_MALE + SA_MIDDLE;
	speakers->Push(data);

	/*
		audios->Push();
		markers->Push();
		divs->Push();
	*/
}

// -- 0
void Rules::RuleEmph(ruleUpdate *ru,string str)
{
	string level;
	GetParameter("level",str,&level);
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	
	if(level.size() != 0)
	{
		if(DEBUG_0) cout<<"\t! Pushing to stack"<<endl;
		if(level == "strong")
			emphs->Push(EL_STRONG);
		else if(level == "moderate")
			emphs->Push(EL_MODERATE);
		else if(level == "none")
			emphs->Push(EL_NONE);
		else if(level == "reduced")
			emphs->Push(EL_REDUCED);
		else
		{
			if(DEBUG_0) cout<<"Non recognized emph level value"<<endl;
			ru->valid = FALSE;
		}
		if(DEBUG_0) cout<<"New emph rule: "<<emphs->TopE()<<endl;
		ru->ruleNo	= emphs->GetRuleno();
		ru->data	= emphs->TopE();
	}
	else
	{
		// default value
		emphs->Push(EL_MODERATE);
		ru->ruleNo	= emphs->GetRuleno();
		ru->data	= emphs->TopE();
	}
	// indicate new rule
}
void Rules::RuleEmphC(ruleUpdate * ru)
{
	if(emphs->GetNoElem() > 1)
	{
		emphs->Pop();
		if(DEBUG_0) cout<<"\t! Removing from stack"<<endl;
	}
	else { ru->valid = FALSE; }
	ru->ruleNo	= emphs->GetRuleno();
	ru->data	= emphs->TopE();
	// indicate new rule
}

// -- 1
void Rules::RuleBreak(ruleUpdate *ru,string str)
{
	string level, msec, type;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;

	GetParameter("level",str,&level);
	GetParameter("type",str,&type);
	
	int data=0;
	if(level.size() != 0)
	{
		if(level == "large")
			data = 10*BL_LARGE;
		else if(level == "medium")
			data = 10*BL_MEDIUM;
		else if(level == "small")
			data = 10*BL_SMALL;
		else if(level == "none")
			data = 10*BL_NONE;
		else
		{
			if(DEBUG_0) cout<<"Non recognized break level value"<<endl;
			ru->valid = FALSE;
		}
	}
	else
	{
		data = 10*BL_MEDIUM;
	}

	if(type.size()	!= 0)
	{
		if(DEBUG_0) cout<<"\tType = "<<type<<endl;
		if(type == "?")
			data += BT_QUEST;
		else if(type == "!")
			data += BT_EXCL;
		else if(type == ".")
			data += BT_DOT;
		else if(type == ",")
			data += BT_COMMA;
		else
		{
			if(DEBUG_0) cout<<"Non recognized break type value"<<endl;
			ru->valid = FALSE;
		}
	}else
	{
		data += BT_DOT;
	}
	if(DEBUG_0) cout<<"New break rule: "<<data<<endl;
	ru->ruleNo = 1;
	ru->data = data;
	// indicate new rule
}

// -- 2
void Rules::RulePitch(ruleUpdate *ru,string str)
{
	string base, middle, range;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;

	GetParameter("base",str,&base);
	GetParameter("middle",str,&middle);
	GetParameter("range",str,&range);
	
	int data=0;

	if(base.size()	!= 0)
	{
		if(base == "highest")
			data += 100* PB_HIGHEST;
		else if(base == "high")
			data += 100* PB_HIGH;
		else if(base == "medium")
			data += 100* PB_MEDIUM;
		else if(base == "low")
			data += 100* PB_LOW;
		else if(base == "lowest")
			data += 100* PB_LOWEST;
		else
		{
			if(DEBUG_0) cout<<"Non recognized pitch base value"<<endl;
		}
	}else
	{
		data += 100* PB_MEDIUM;
	}

	if(middle.size()!= 0)
	{
		if(middle == "highest")
			data += 10* PB_HIGHEST;
		else if(middle == "high")
			data += 10* PB_HIGH;
		else if(middle == "medium")
			data += 10* PB_MEDIUM;
		else if(middle == "low")
			data += 10* PB_LOW;
		else if(middle == "lowest")
			data += 10* PB_LOWEST;
		else
		{
			if(DEBUG_0) cout<<"Non recognized pitch middle value"<<endl;
		}
	}else
	{
		data += 10* PB_MEDIUM;
	}

	if(range.size() != 0)
	{
		if(range == "highest")
			data += PB_HIGHEST;
		else if(range == "high")
			data += PB_HIGH;
		else if(range == "medium")
			data += PB_MEDIUM;
		else if(range == "low")
			data += PB_LOW;
		else if(range == "lowest")
			data += PB_LOWEST;
		else
		{
			if(DEBUG_0) cout<<"Non recognized pitch middle value"<<endl;
			ru->valid = FALSE;
		}
	}
	else
	{
		data += PB_MEDIUM;
	}

	if(DEBUG_0) cout<<"New pitch rule: "<<data<<endl;
	if(DEBUG_0) cout<<"\tPushing to stack"<<endl;
	pitchs->Push(data);
	ru->ruleNo = pitchs->GetRuleno();
	ru->data = data;
	// indicate new rule
}
void Rules::RulePitchC(ruleUpdate * ru)
{
	if(pitchs->GetNoElem() > 1)
	{
		pitchs->Pop();
		if(DEBUG_0) cout<<"\t! Removing from stack"<<endl;
	}
	else { ru->valid = FALSE; }
	// indicate new rule
	ru->ruleNo	= pitchs->GetRuleno();
	ru->data	= pitchs->TopE();
}
// -- 3
void Rules::RuleRate(ruleUpdate *ru,string str)
{
	string speed;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	GetParameter("speed",str,&speed);
	int data;
	if(speed.size() != 0)
	{
		if(DEBUG_0) cout<<"\tSpeed = "<<speed<<endl;
		if(speed == "fastest")
			data = RS_FASTEST;
		else if(speed == "fast")
			data = RS_FAST;
		else if(speed == "medium")
			data = RS_MEDIUM;
		else if(speed == "slow")
			data = RS_SLOW;
		else if(speed == "slowest")
			data = RS_SLOWEST;
		else 
		{
			if(DEBUG_0) cout<<"Non recognized speed value"<<endl;
			ru->valid = FALSE;
		}
	}
	else
	{
		data = RS_MEDIUM;
	}

	if(DEBUG_0) cout<<"New rate rule: "<<data<<endl;
	if(DEBUG_0) cout<<"\tPushing to stack"<<endl;
	rates->Push(data);
	ru->ruleNo = rates->GetRuleno();
	ru->data = data;
}

void Rules::RuleRateC(ruleUpdate * ru)
{
	if(rates->GetNoElem() > 1)
	{
		rates->Pop();
		if(DEBUG_0) cout<<"\t! Removing from stack"<<endl;
	}
	else { ru->valid = FALSE; }
	// indicate new rule
	ru->ruleNo	= rates->GetRuleno();
	ru->data	= rates->TopE();
}

// -- 4
void Rules::RuleVolume(ruleUpdate *ru,string str)
{
	string level;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	GetParameter("level",str,&level);
	int data;

	if(level.size() != 0)
	{
		if(DEBUG_0) cout<<"\tlevel = "<<level<<endl;
	
		if(level == "loudest")
			data = VL_LOUDEST;
		else if(level == "loud")
			data = VL_LOUD;
		else if(level == "medium")
			data = VL_MEDIUM;
		else if(level == "quiet")
			data = VL_QUIET;
		else 
		{
			if(DEBUG_0) cout<<"Non recognized volume level value"<<endl;
			ru->valid = FALSE;
		}
	}
	else
	{
		data = VL_MEDIUM;
	}
	//if(DEBUG_0) 
		cout<<"New volume rule: "<<data<<endl;
	//if(DEBUG_0) 
		cout<<"\tPushing to stack"<<endl;
	volumes->Push(data);
	ru->ruleNo = volumes->GetRuleno();
	ru->data = data;
	ru->displayMe();
}
void Rules::RuleVolumeC(ruleUpdate * ru)
{
	if(volumes->GetNoElem() > 1)
	{
		volumes->Pop();
		if(DEBUG_0) cout<<"\t! Removing from stack"<<endl;
	}
	else { ru->valid = FALSE; }
	// indicate new rule
	ru->ruleNo	= volumes->GetRuleno();
	ru->data	= volumes->TopE();
}
// -- 5
void Rules::RuleEngine(ruleUpdate *ru,string str)
{
	string id;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	GetParameter("id",str,&id);
	
	if(id.size() != 0)
	{
		if(DEBUG_0) cout<<"\tID = "<<id<<endl;
		int data = 0;
		if(id == "syllable")
			data = EI_SY;
		else if(id == "phonem")
			data = EI_PH;
		else 
		{
			if(DEBUG_0) cout<<"Non recognized engine id value"<<endl;
			ru->valid = FALSE;
		}
		if(DEBUG_0) cout<<"New engine rule: "<<data<<endl;
		if(DEBUG_0) cout<<"\tPushing to stack"<<endl;
		engines->Push(data);
		ru->ruleNo = engines->GetRuleno();
		ru->data = data;
	}
}
void Rules::RuleEngineC(ruleUpdate * ru)
{
	if(engines->GetNoElem() > 1)
	{
		engines->Pop();
		if(DEBUG_0) cout<<"\t! Removing from stack"<<endl;
	}
	else { ru->valid = FALSE; }
	// indicate new rule
	ru->ruleNo	= engines->GetRuleno();
	ru->data	= engines->TopE();
}
// -- 6
void Rules::RuleSable()
{
	if(DEBUG_0) cout<<"I recognize Sable"<<endl;
	this->sableOn = TRUE;
}
void Rules::RuleSableC()
{
	this->sableOn = FALSE;
}
// -- 7
void Rules::RulePron(ruleUpdate *ru,string str)
{
	string sub;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	GetParameter("sub",str,&sub);

	if(sub.size() != 0)
	{
		if(DEBUG_0) cout<<"\tSUB = "<<sub<<endl;
		this->pronounce = sub;
	}
}

// -- 8
void Rules::RuleSayas(ruleUpdate *ru,string str)
{
	string mode, modetype;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	GetParameter("mode",str,&mode);
	GetParameter("modetype",str,&modetype);
	int data=0;

	if(mode.size() != 0)
	{
		if(DEBUG_0) cout<<"\tMode = "<<mode<<endl;
	}
	if(modetype.size() != 0)
	{
		if(DEBUG_0) cout<<"\tMode type = "<<modetype <<endl;
	}
}
void Rules::RuleSayasC(ruleUpdate * ru)
{

}
// -- 9
void Rules::RuleLang(ruleUpdate *ru,string str)
{
	string id;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	GetParameter("id",str,&id);

	if(id.size() != 0)	
	{
		if(DEBUG_0) cout<<"\tID = "<<id<<endl;
		int data;
		if(id == "tr")
			data = LI_TR;
		else
		{
			if(DEBUG_0) cout<<"Non recognized lang id value"<<endl;
			ru->valid = FALSE;
		}
		if(DEBUG_0) cout<<"New lang rule: "<<data<<endl;
		if(DEBUG_0) cout<<"\tPushing to stack"<<endl;
		langs->Push(data);
		ru->ruleNo = langs->GetRuleno();
		ru->data = data;
	}
}
void Rules::RuleLangC(ruleUpdate * ru)
{
	if(langs->GetNoElem() > 1)
	{
		langs->Pop();
		if(DEBUG_0) cout<<"\t! Removing from stack"<<endl;
	}
	else { ru->valid = FALSE; }
	// indicate new rule
	ru->ruleNo	= langs->GetRuleno();
	ru->data	= langs->TopE();
}

// -- 10
void Rules::RuleSpeaker(ruleUpdate *ru,string str)
{
	string gender, age;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	GetParameter("gender",str,&gender);
	GetParameter("age",str,&age);
	int data=0;
	if(gender.size() != 0)
	{
		if(DEBUG_0) cout<<"\tGENDER = "<<gender<<endl;
		if(gender == "male")
			data += 10*SG_MALE;
		else if(gender == "female")
			data += 10*SG_FEMALE;
		else
			if(DEBUG_0) cout<<"Non recognized gender spec"<<endl;
	}
	if(age.size() != 0)	
	{
		if(DEBUG_0) cout<<"\tAGE = "<<age<<endl;
		if(age == "older")
			data += SA_OLDER;
		else if(age == "middle")
			data += SA_MIDDLE;
		else if(age == "younger")
			data += SA_YOUNGER;
		else if(age == "teen")
			data += SA_TEEN;
		else if(age == "child")
			data += SA_CHILD;
		else
		{
			if(DEBUG_0) cout<<"Non recognized age spec"<<endl;
			ru->valid = FALSE;
		}
	}
	if(DEBUG_0) cout<<"New speaker rule: "<<data<<endl;
	if(DEBUG_0) cout<<"\tPushing to stack"<<endl;
	speakers->Push(data);
	ru->ruleNo = speakers->GetRuleno();
	ru->data = data;
}
void Rules::RuleSpeakerC(ruleUpdate * ru)
{
	if(speakers->GetNoElem() > 1)
	{
		speakers->Pop();
		if(DEBUG_0) cout<<"\t! Removing from stack"<<endl;
	}
	else { ru->valid = FALSE; }
	// indicate new rule
	ru->ruleNo	= speakers->GetRuleno();
	ru->data	= speakers->TopE();
}

// -- 11
Word * Rules::RulePronClosable(string str)
{
	string sub;
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	GetParameter("sub",str,&sub);

	if(DEBUG_0) cout<<"\tSUB = "<<sub<<endl;
	this->pronouncec = sub;
	Word *insert = new Word("");
	insert->Split(sub);
	return insert;
}
// -- 11
void Rules::RulePronClosableC()
{
	if(DEBUG_0) cout<<"close rulepronc"<<endl;
	this->pronouncec = "";
}
// -- 12
void Rules::RuleAudio(ruleUpdate *ru,string str)
{
	// not yet
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	ru->valid = FALSE;
}
void Rules::RuleAudioC(ruleUpdate * ru)
{

}
// -- 13
void Rules::RuleMarker(ruleUpdate *ru,string str)
{
	// not yet
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	ru->valid = FALSE;
}
void Rules::RuleMarkerC(ruleUpdate * ru)
{

}
// -- 14
void Rules::RuleDiv(ruleUpdate *ru,string str)
{
	// not yet
	if(DEBUG_0) cout<<"I recognize ( "<<str<<" )"<<endl;
	ru->valid = FALSE;
}
void Rules::RuleDivC(ruleUpdate * ru)
{

}