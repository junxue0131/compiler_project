
  #include "caculate.h"
  #include "globals.h"
  /*构造符号表  符号表有id、类型、值
  //其他操作就都是针对符号表的操作*/
  static double cExp(TreeNode * );

  typedef struct st
  { 
    ExpType type;
    double value;
    char * id;
    struct st * next;
  } * symtabStruct;

  static symtabStruct symtab = NULL;

  static symtabStruct findSym(char * str){
    symtabStruct p = symtab;
    while(p!=NULL){
      if (strcmp(str,p->id) == 0)
        return p;/*返回指针，让调用方自己取*/
      p = p->next;
    }
    printf("'%s' not define\n", str);
    exit(0);
  }

  static int findSym2(char * str){
    symtabStruct p = symtab;
    while(p!=NULL){
      if (strcmp(str,p->id) == 0)
        return 1;
      p = p->next;
    }
    return 0;
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
      return cExp(tree->child[0]) + cExp(tree->child[1]); 
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

  static double cExp(TreeNode * tree)/*我就返回一个double，让调用的地方做类型转换*/
  {
    switch (tree->kind.exp) {
          case OpK:
            return cOp(tree);
            break;
          case IdK:
            {symtabStruct s = findSym(tree->attr.name);
            return s->value;}
            break;
          case ConstIntK:
            return tree->attr.val;
            break;
          case ConstRealK:
            return tree->attr.val;
            break;
          case ConstCharK:
            return tree->attr.val;
          default:
            printf( "Unknown ExpNode kind\n");
            break;
        }
  }

  static int realin = 0;
  static void realIn(TreeNode * tree)
  { if (tree != NULL){
      if (tree->kind.exp == ConstRealK)
        realin = 1;
      if (tree->kind.exp == IdK){/*需要检查id中是否有real*/
        symtabStruct s = findSym(tree->attr.name);
        if (s->type == Reall)
        {
          realin = 1;
        }
      }
      int i;
      for (i=0;i<MAXCHILDREN;i++)
           realIn(tree->child[i]);
    }
    return ;
  }

  static int notcharin = 0;
  static void notcharIn(TreeNode * tree)
  { if (tree != NULL){
      if (tree->kind.exp != ConstCharK)
        notcharin = 1;
      if (tree->kind.exp == IdK){/*需要检查id中是否有real*/
        symtabStruct s = findSym(tree->attr.name);
        if (s->type != Charac)
        {
          notcharin = 1;
        }
      }
      int i;
      for (i=0;i<MAXCHILDREN;i++)
           notcharIn(tree->child[i]);
    }
    return ;
  }

  static int charin = 0;
  static void charIn(TreeNode * tree)
  { if (tree != NULL){
      if (tree->kind.exp == ConstCharK)
        charin = 1;
      if (tree->kind.exp == IdK){/*需要检查id中是否有real*/
        symtabStruct s = findSym(tree->attr.name);
        if (s->type == Charac)
        {
          charin = 1;
        }
      }
      int i;
      for (i=0;i<MAXCHILDREN;i++)
           charIn(tree->child[i]);
    }
    return ;
  }

  void caculate(TreeNode * tree)
  { 
    while (tree != NULL) {
      if (tree->nodekind==StmtK)
      { switch (tree->kind.stmt) {
          case IfK:
            if ((int)cExp(tree->child[0])){
              caculate(tree->child[1]);
            }else{
              caculate(tree->child[2]);
            }
            break;
          case RepeatK:
            while(!(int)cExp(tree->child[1])){
              caculate(tree->child[0]);
            }
            break;
          case AssignK:
            {/*类型检查 不能给int或char赋值real*/
            symtabStruct s = findSym(tree->attr.name);
            realIn(tree->child[0]);
            if(realin && (s->type==Integer||s->type==Charac)){
              printf("'%s' can't be assigned real value\n", tree->attr.name);
              exit(0);
            }
            realin = 0;
            /*需要检查char问题，对一个char类型，表达式必须全是char*/
            notcharIn(tree->child[0]);
            if(notcharin && s->type==Charac){
              printf("'%s' can't be assigned not char value\n", tree->attr.name);
              exit(0);
            }
            notcharin = 0;
            /*不能给int或real赋值char*/
            charIn(tree->child[0]);
            if(charin && (s->type==Integer||s->type==Reall)){
              printf("'%s' can't be assigned char value\n", tree->attr.name);
              exit(0);
            }
            charin = 0;
            printf("assigned %f to '%s'\n", cExp(tree->child[0]), tree->attr.name);
            s->value = cExp(tree->child[0]);}
            break;
          case ReadK:
          { symtabStruct s = findSym(tree->attr.name);
            int i;
            char c;
            double d;
            if (s->type == Reall){
              printf("input '%s':", tree->attr.name);
              scanf("%lf", &d);
              s->value = d;
            }
            else if(s->type == Charac){
              printf("input '%s':", tree->attr.name);
              scanf("%c", &c);
              while (c == 10){
                scanf("%c", &c);
              }
              i = (int)c;
              s->value = (double)i;
            }
            else{
              printf("input '%s':", tree->attr.name);
              scanf("%d", &i);
              s->value = (double)i;
            }
          }
            break;
          case WriteK:
          { 
            TreeNode * s = tree->child[0];
            switch (s->kind.exp) {
              case OpK:
                printf("write: %f\n",  cOp(s));
                break;
              case IdK:
                {
                  printf("write '%s':\n", s->attr.name);
                  symtabStruct st = findSym(s->attr.name);
                  if (st->type == Reall){
                    printf("%f\n", st->value);
                  }
                  else if(st->type == Charac){
                    int i = (int)st->value;                  
                    printf("%c\n", (char)i);
                  }
                  else{
                    printf("%d\n", (int)st->value);
                  }
                }
                break;
              case ConstIntK:
                printf("write: %d\n", (int)s->attr.val);
                break;
              case ConstRealK:
                printf("write: %f\n", s->attr.val);
                break;
              case ConstCharK:
              {
                int i = (int)s->attr.val;
                printf("write: %c\n", (char)i);
              }
              break;
              default:
                printf( "Unknown ExpNode kind\n");
                break;
            }
            
          }
            break;
          default:
            printf( "Unknown ExpNode kind\n");
            break;
        }
      }
      else if (tree->nodekind==DecK)
      { 
        switch (tree->kind.exp) {
          case IntK:
            if (symtab == NULL){
              symtab = (symtabStruct) malloc(sizeof(struct st));
              symtab->type = Integer;
              symtab->value = 0;/*初始化为0*/
              symtab->id = tree->attr.name;
              symtab->next = NULL;
            }
            else{
              if (findSym2(tree->attr.name)){
                printf("'%s' has already been defined\n", tree->attr.name);
                exit(0);
              }

              symtabStruct t = (symtabStruct) malloc(sizeof(struct st));
              symtabStruct p = symtab;
              while(p->next != NULL){
                p = p->next;
              }
              p->next = t;
              t->type = Integer;
              t->value = 0;
              t->id = tree->attr.name;
              t->next = NULL;
            }
            break;
          case RealK:
            if (symtab == NULL){
              symtab = (symtabStruct) malloc(sizeof(struct st));
              symtab->type = Reall;
              symtab->value = 0;
              symtab->id = tree->attr.name;
              symtab->next = NULL;
            }
            else{
              if (findSym2(tree->attr.name)){
                printf("'%s' has already been defined\n", tree->attr.name);
                exit(0);
              }

              symtabStruct t = (symtabStruct) malloc(sizeof(struct st));
              symtabStruct p = symtab;
              while(p->next != NULL){
                p = p->next;
              }
              p->next = t;
              t->type = Reall;
              t->value = 0;
              t->id = tree->attr.name;
              t->next = NULL;
            }
            break;
          case CharK:
            if (symtab == NULL){
              symtab = (symtabStruct) malloc(sizeof(struct st));
              symtab->type = Charac;
              symtab->value = 0;
              symtab->id = tree->attr.name;
              symtab->next = NULL;
            }
            else{
              if (findSym2(tree->attr.name)){
                printf("'%s' has already been defined\n", tree->attr.name);
                exit(0);
              }

              symtabStruct t = (symtabStruct) malloc(sizeof(struct st));
              symtabStruct p = symtab;
              while(p->next != NULL){
                p = p->next;
              }
              p->next = t;
              t->type = Charac;
              t->value = 0;
              t->id = tree->attr.name;
              t->next = NULL;
            }
            break;
          default:
            printf("Unknown DecNode kind\n");
            break;
        }
      }
      else if (tree->nodekind==PromK)
      {
        int i;
        for (i=0;i<MAXCHILDREN;i++)
           caculate(tree->child[i]);
      }
      else printf( "Unknown node kind\n");
      tree = tree->sibling;
    }
  }