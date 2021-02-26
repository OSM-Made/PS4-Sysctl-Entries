#include <sys/param.h>
#include <sys/sysctl.h>

#define    CTLTYPE        0xf    /* mask for the type */
#define    CTLTYPE_NODE    1    /* name is a node */
#define    CTLTYPE_INT    2    /* name describes an integer */
#define    CTLTYPE_STRING    3    /* name describes a string */
#define    CTLTYPE_S64    4    /* name describes a signed 64-bit number */
#define    CTLTYPE_OPAQUE    5    /* name describes a structure */
#define    CTLTYPE_STRUCT    CTLTYPE_OPAQUE    /* name describes a structure */
#define    CTLTYPE_UINT    6    /* name describes an unsigned integer */
#define    CTLTYPE_LONG    7    /* name describes a long */
#define    CTLTYPE_ULONG    8    /* name describes an unsigned long */
#define    CTLTYPE_U64    9    /* name describes an unsigned 64-bit number */
#define    CTLTYPE_U8    0xa    /* name describes an unsigned 8-bit number */
#define    CTLTYPE_U16    0xb    /* name describes an unsigned 16-bit number */
#define    CTLTYPE_S8    0xc    /* name describes a signed 8-bit number */
#define    CTLTYPE_S16    0xd    /* name describes a signed 16-bit number */
#define    CTLTYPE_S32    0xe    /* name describes a signed 32-bit number */
#define    CTLTYPE_U32    0xf    /* name describes an unsigned 32-bit number */

#define CTLFLAG_RD	0x80000000	/* Allow reads of variable */
#define CTLFLAG_WR	0x40000000	/* Allow writes to the variable */
#define CTLFLAG_RW	(CTLFLAG_RD|CTLFLAG_WR)
#define CTLFLAG_NOLOCK	0x20000000	/* XXX Don't Lock */
#define CTLFLAG_ANYBODY	0x10000000	/* All users can set this var */
#define CTLFLAG_SECURE	0x08000000	/* Permit set only if securelevel<=0 */

SLIST_HEAD(sysctl_oid_list, sysctl_oid);

struct sysctl_oid {
	struct sysctl_oid_list *oid_parent; //0x0
	SLIST_ENTRY(sysctl_oid) oid_link; //0x8
	int		oid_number; //0x10
	int		oid_kind; //0x14
	void		*oid_arg1;
	int		oid_arg2;
	const char	*oid_name;
	int 		(*oid_handler);
	const char	*oid_fmt;
};

void PrintNode(sysctl_oid *Node, const char* Name)
{
  //Loop through node oid till there isnt a next one.
	while(Node != NULL)
	{
		printf("%s.%s Type:%X \n", Name, Node->oid_name, Node->oid_kind & CTLTYPE);
    
    //If the oid is a node recursively print each node.
		if((Node->oid_kind & CTLTYPE) == CTLTYPE_NODE)
		{
			char NodeNameFull[0x200];
			sprintf(NodeNameFull, "%s.%s", Name, Node->oid_name);
      
			PrintNode(SLIST_FIRST((sysctl_oid_list*)Node->oid_arg1), (const char*)NodeNameFull);
		}

		//Enter Next oid
		Node = SLIST_NEXT(Node, oid_link);
	}
}

void DumpSysCtl()
{
	sysctl_oid_list* sysctl__children = (sysctl_oid_list*)resolve(0x22B48F0);
	struct sysctl_oid *oid = SLIST_FIRST(sysctl__children);
  
  //Loop through oid till there isnt a next one.
	while(oid != NULL)
	{
		printf("%s T:%X \n", oid->oid_name, oid->oid_kind & CTLTYPE);
    
    //If the oid is a node recursively print each node.
		if((oid->oid_kind & CTLTYPE) == CTLTYPE_NODE)
			PrintNode(SLIST_FIRST((sysctl_oid_list*)oid->oid_arg1), oid->oid_name);

		//Enter Next oid
		oid = SLIST_NEXT(oid, oid_link);
	}
}
