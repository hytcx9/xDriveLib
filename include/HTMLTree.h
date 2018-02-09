
#ifndef __ZHUtil__HTMLTree__
#define __ZHUtil__HTMLTree__
#define			XDRIVE_API			__declspec(dllexport)
#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4067)

#include <vector>
#include <string>
#include <map>
using namespace std;

/*
 define callback function type here. used if parsing procedure.
 init by null , check it if null when use
 */

// call back to tell connect ok or not
/*
  return 1 ,continue parse, 0 will break parsing.
  if break parse, HTMLBranch will be deleted for the dealloc function.
  so , note to copy data out then return 0.
 */
class XDRIVE_API HTMLBranch;
typedef int(*ptrCallBackFoundTag)(void* sender, HTMLBranch* branch);
typedef int(*ptrCallBackFoundJS)(void* sender, HTMLBranch* brh);
/*
 * some structures need. 
 */
#define			ATTRIBUTE_NAME_LEN		32
#define			BRANCH_NAME_LEN			33
// the attributes structure, save value and name, and will be push_back into vector
typedef struct attribute
{
	wchar_t attr_name[ATTRIBUTE_NAME_LEN];
	wchar_t* attr_value;
	attribute():attr_value(NULL)
	{
		//memset(attr_name, 0, ATTRIBUTE_NAME_LEN);
		wmemset(attr_name, 0x0, ATTRIBUTE_NAME_LEN);
	}
}ATTRIBUTE;

// a link nodes stucture to get target Branch
// this stucture is for parse the path later use.
typedef struct path_nodes
{
	wchar_t* path_name;	// such as aa
	path_nodes* next;	// pointer to next path_node
	path_nodes():path_name(NULL),next(NULL)
	{
	}
}PATH_NODES;

class XDRIVE_API HTMLBranch;
class XDRIVE_API HTMLLeaf;

typedef HTMLBranch*  t_pHTMLBranch;
typedef struct _HTMLBranch_list 
{
	HTMLBranch* node;
	int index;
	_HTMLBranch_list* next;
	_HTMLBranch_list():node(NULL), index(0), next(NULL)
	{

	}
}HTMLBranch_list;


class XDRIVE_API HTMLLeaf
{
public:
	HTMLLeaf(const wchar_t* lf = NULL);
	~HTMLLeaf(void);
	wchar_t* pLeafValue;
	wchar_t* pLeafPath;
	void setLeaf(const wchar_t* lf = NULL);
	HTMLBranch* getParent() { return _parent; };
	void setParent(HTMLBranch* brch) { _parent = brch; };

	HTMLBranch* _parent;
};


class XDRIVE_API HTMLTree
{
public:
	HTMLTree(void);
	virtual ~HTMLTree(void);
	virtual bool addBranch(HTMLBranch* brh) = 0;
	virtual bool removeBranch(HTMLBranch* brh) = 0;
	virtual HTMLBranch* parseBranches(wchar_t* inStr, wchar_t* basePath = NULL) = 0;
	virtual wstring getHTMLString(wstring& ss) = 0;
	virtual wstring getFormatHTMLString(int deep = 0) = 0;
    virtual wstring innerHTMLStr(wstring& ss) = 0;
	virtual wstring innerText() = 0;
	// static funcs
	static bool isUsableChar(wchar_t inc);
	/************************************************************************/
	/* HTMLBranch* tree = HTMLTree::getHTMLTree(ANSIToUnicode(pClient->html));
	/*
	/************************************************************************/
	static HTMLBranch* getHTMLTree(wchar_t* inStr, bool verbose = false);
    virtual HTMLBranch* getBranchByAttrname(wchar_t* attrName, wchar_t* attrValue = NULL) = 0;
    
public:
	bool verbose;	// print info if true
	int found_branches;
	HTMLBranch* pChild_branches;
	int nChildBranchNum;
	int depth; // or \t nums , the root has depth=0
	//t_pHTMLBranch b_list[1000];// 100 HTMLBranch pointer elements array, or like :
	// HTMLBranch* b_list[100];
	HTMLBranch_list* queryList;

	HTMLBranch* getParent() { return _parent; };
	void setParent(HTMLBranch* brch) { _parent = brch;  };

	HTMLBranch* _parent;
};

class XDRIVE_API HTMLBranch : public HTMLTree
{
public:
	HTMLBranch(void);  
	HTMLBranch(wchar_t* path, wchar_t* v = NULL); // create branch by path
	HTMLBranch(PATH_NODES* name, wchar_t* leafValue = NULL, int deep = 0);

	void		InitBranch();
	virtual		~HTMLBranch(void);
	void		addLeaf(HTMLLeaf* xl);
	void		addBufferAsLeaf();
	virtual bool	addBranch(HTMLBranch* brh);
	virtual bool	removeBranch(HTMLBranch* brh);
	virtual HTMLBranch* parseBranches(wchar_t* inStr, wchar_t* basePath = NULL);
	wstring innerHTMLStr(wstring& ss);
	// return number of got branches, branches saved in inList .
	int			get_all_branches(wchar_t* path,
                                 HTMLBranch** &inList,
                                 wchar_t* attrName = NULL,
                                 wchar_t* attrValue = NULL);
	int			free_lists(HTMLBranch** &inList, int size=0);// free the mem of lists which have HTMLBranch* nodes
	
	HTMLBranch_list* get_branch(HTMLBranch_list* nl = NULL, PATH_NODES* cur_node = NULL, wchar_t* attrName = NULL, wchar_t* attrValue = NULL);

	bool		hasAttributeName(wchar_t* attrName); // if it has an attrName
	wchar_t*	getAttribute(wchar_t* attrName);
	bool		setAttribute(wchar_t* name, wchar_t* value);
	wstring		getHTMLString(wstring& ss);
	wstring		getFormatHTMLString(int deep = 1);
	bool		free_path_nodes(PATH_NODES* head);
	wchar_t*	getLeaf(void);
	bool		setLeaf(wchar_t* inStr);
	HTMLBranch* createBranches(wchar_t* path = NULL, wchar_t* leafValue = NULL, int deep = 0);
	PATH_NODES* getPathNodes(wchar_t* path);
	//string getLastPath(PATH_NODES* all_nodes, char* whole_path);
	wstring		innerText();
    /** get branches by given tagName
     *  if recurve , then loop into it for all , and put in array back
     */
	void		getBranchesByTagName(wchar_t* tagName, vector <HTMLBranch*>& list, wchar_t* attrName = NULL, wchar_t* attrV = NULL, bool entry = true);
    
    HTMLBranch* getBranchByAttrname(wchar_t* attrName, wchar_t* attrValue = NULL);
    
public:
	ptrCallBackFoundTag ptrCallBack_htmlTag;
	ptrCallBackFoundJS ptrCallBack_foundjs;

	wchar_t brch_name[BRANCH_NAME_LEN];		// must less than 31 chars .\0 is one
	wchar_t* brch_path; // 'a/b/c/d....'		
	vector <ATTRIBUTE> attrs;
	vector <ATTRIBUTE>::iterator itorAttr;
	wchar_t brch_tag_start[BRANCH_NAME_LEN + 1]; // '<' added
	wchar_t brch_tag_end[BRANCH_NAME_LEN + 3]; // "</" and ">"
	wchar_t* strBuffer;

	int depth; // or \t nums , the root has depth=0

	bool hasAttributes;
	int nChildBranchNum;
	int nBrothersNum;
	bool hasLeaf;

	HTMLBranch* pChild_branches;
	HTMLBranch* sibling_pre;
	HTMLBranch* sibling_next;
	HTMLLeaf* leaf;

};


#endif


