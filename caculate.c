
#include "caculate.h"
#include "globals.h"
//构造符号表  符号表有id、类型、值
//其他操作就都是针对符号表的操作

typedef struct st
{ 
  ExpType type;
  double value;
  char * id;
  struct st * next;
} * Symtab;

static Symtab symtab = NULL;

static Symtab findSym(char * str){
  Symtab p = symtab;
  while(p!=NULL){
    if (strcmp(str,p->id) == 0)
      return p;//返回指针，让调用方自己取
    p = p->next;
  }
  printf("not define");
  Error = TRUE;
}

static double cOp(TreeNode * tree)
{
  switch(tree->attr.op)
  {
    case LT: 
    if(cExp(tree->child[0])<cExp(tree->child[1]))
        return 1;
    else
      return 0;
    break;
    case EQ: 
    if(cExp(tree->child[0])==cExp(tree->child[1]))
        return 1;
    else
      return 0;
    break;
    case PLUS: 
    return cExp(tree->child[0]) + cExp( cExp(tree->child[1])); 
    break;
    case MINUS:
    return  cExp(tree->child[0]) -  cExp(tree->child[1]); 
    break;
    case TIMES: 
    return  cExp(tree->child[0]) *  cExp(tree->child[1]); 
    break;
    case OVER: 
    return  cExp(tree->child[0]) /  cExp(tree->child[1]); 
    break;
  }
}

static double cExp(TreeNode * tree)//我就返回一个double，让调用的地方做类型转换
{
  switch (tree->kind.exp) {
        case OpK:
          return cOp(tree);
          break;
        case IdK:
          Symtab s = findSym(tree->attr.name);
          return s->value;
          break;
        case ConstIntK:
          return tree->attr.val;
          break;
        case ConstRealK:
          return tree->attr.val;
          break;
        //看大橙子怎么存char
        default:
          fprintf( "Unknown ExpNode kind\n");
          break;
      }
}

static int realin = 0;
static void realIn(TreeNode * tree)
{ if (tree != NULL){
    if (tree->kind.exp == ConstRealK)
      realin = 1;
    if (tree->kind.exp == IdK){//需要检查id中是否有real
      Symtab s = findSym(tree->attr.name);
      if (s->type == Reall)
      {
        realin = 1;
      }
    }
    for (i=0;i<MAXCHILDREN;i++)
         realin(tree->child[i]);
  }
  return ;
}

void caculate(TreeNode * syntaxTree)
{ 
  // Child = TRUE;
  while (tree != NULL) {
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          if (int(cExp(tree->child[0]))){
            caculate(tree->child[1]);
          }else{
            caculate(tree->child[2]);
          }
          // Child = FALSE;
          break;
        case RepeatK:
          while(! int(cExp(tree->child[1]))){
            caculate(tree->child[0]);
          }
          break;
        case AssignK://感觉这里可以检查类型
          Symtab s = findSym(tree->attr.name);
          realin(tree->child[0]);//类型检查 不能给int或char赋值real
          if(realIn && s->type==Integer||s->type==Char){
            printf("%s can't be assigned real value\n", tree->attr.name);
          Error = TRUE;
          }
          realin = 0;
          //需要检查char问题，对一个char类型，表达式必须全是char

          //

          s->value = cExp(tree->child[0]);
          break;
        case ReadK:
          Symtab s = findSym(tree->attr.name);
          int i;
          char c;
          double d;
          if (s->type == Reall){
            scanf("%f", &d);
            s->value = d;
          }
          else if(s->type == Char){
            scanf("%c", &c);
            i = (int)char;
            s->value = double(i);
          }
          else{
            scanf("%d", &i);
            s->value = double(i);
          }
          break;
        case WriteK:
          Symtab s = findSym(tree->attr.name);
          int i;
          double d;
          if (s->type == Reall){
            printf("%f\n", s->value);
          }
          else if(s->type == Char){
            i = (int)s->value;
            printf("%c\n", i);
          }
          else{
            i = (int)s->value;
            printf("%d\n", i);
          }
          break;
        default:
          fprintf( "Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==DecK)
    { switch (tree->kind.exp) {
        case IntK:
          if (symtab == NULL){
            symtab->ExpType = Integer;
            symtab->value = 0;//初始化为0
            symtab->id = tree->attr.name;
          }
          else{
            Symtab t = (Symtab) malloc(sizeof(struct Symtab));
            Symtab p = symtab;
            while(p->next != NULL){
              p = p->next;
            }
            p->next = t;
            t->ExpType = Integer;
            t->value = 0;//初始化为0
            t->id = tree->attr.name;
          }
          break;
        case RealK:
          if (symtab == NULL){
            symtab->ExpType = Reall;
            symtab->value = 0;//初始化为0
            symtab->id = tree->attr.name;
          }
          else{
            Symtab t = (Symtab) malloc(sizeof(struct Symtab));
            Symtab p = symtab;
            while(p->next != NULL){
              p = p->next;
            }
            p->next = t;
            t->ExpType = Reall;
            t->value = 0;//初始化为0
            t->id = tree->attr.name;
          }
          break;
        case CharK:
          if (symtab == NULL){
            symtab->ExpType = Char;
            symtab->value = 0;//初始化为0
            symtab->id = tree->attr.name;
          }
          else{
            Symtab t = (Symtab) malloc(sizeof(struct Symtab));
            Symtab p = symtab;
            while(p->next != NULL){
              p = p->next;
            }
            p->next = t;
            t->ExpType = Char;
            t->value = 0;//初始化为0
            t->id = tree->attr.name;
          }
          break;
        default:
          fprintf( "Unknown DecNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==PromK)
    {
      fprintf("Program begin:\n");
    }
    else fprintf( "Unknown node kind\n");

    // int i;
    // if (Child)//这好像是不需要的
    //   for (i=0;i<MAXCHILDREN;i++)
    //        caculate(tree->child[i]);
    tree = tree->sibling;
  }
}