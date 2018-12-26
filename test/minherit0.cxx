
#include <stdio.h>
class TObject {
};

class TAttLine {
      public:
	virtual 
		void Modify() { printf("TAttLine::Modify()\n"); }
};

class TLine : public TObject , public TAttLine {
};

class TAttArea {
      public:
	virtual 
		void Modify() { printf("TAttArea::Modify()\n"); }
};

class TBox : public TLine 
	, public TAttArea 
{
      public:
	virtual 
		void Modify() { printf("TBox::Modify()\n"); }
};


int main()
{
	TBox box;
	TLine line;
	TAttArea attarea;

	box.Modify();
	box.TAttLine::Modify();
	box.TAttArea::Modify();
	line.Modify();
	attarea.Modify();

	TLine *pline;
	TAttArea *pattarea;

	printf("virtual resolution\n");

	pline = &box;
	pline->Modify();
	pline = &line;
	pline->Modify();

	pattarea = &box;
	pattarea->Modify();
	pattarea = &attarea;
	pattarea->Modify();

	return 0;
}
