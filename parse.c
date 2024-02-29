#include "rvcc.h"

// expr = equality 相等性的判断
// equality = relational ("==" relational | "!=" relational)* 判断里面有很多关系运算
// relational = add ("<" add |"<=" add | ">" add |">=" add)* 先相等、不等，在大小判断。优先级
// add = mul ("+" mul | "-" mul)*
// mul = primary ("*" unary | "/" unary)*
// unary = ("+" | "-") unary | primary
// 乘数由一元运算数构成，而一元运算数前面可能带有加号或者减号（多个）
// primary = "(" expr ")" | num
static Node *expr(Token **Rest, Token *Tok);
static Node *equality(Token **Rest, Token *Tok);
static Node *relational(Token **Rest, Token *Tok);
static Node *add(Token **Rest, Token *Tok);
static Node *mul(Token **Rest, Token *Tok);
static Node *unary(Token **Rest, Token *Tok);
static Node *primary(Token **Rest, Token *Tok);

// 新建一个节点
static Node *newNode(NodeKind Kind){
    Node *Nd = calloc(1, sizeof(Node));
    Nd->Kind = Kind;
    return Nd;
}

// 新建一个单叉树，因为数字前的多个'-'号在构成二叉树的时候不需要左右分支
static Node *newUnary(NodeKind Kind, Node *Expr){
    Node *Nd = newNode(Kind);
    Nd->LHS = Expr;
    return Nd;
}

// 新建一个二叉树节点
static Node *newBinary(NodeKind Kind, Node *LHS, Node *RHS){
    Node *Nd = newNode(Kind);
    Nd->LHS = LHS;
    Nd->RHS = RHS;
    return Nd;
}

// 新建一个数字节点
static Node *newNum(int Val){
    Node *Nd = newNode(ND_NUM);
    Nd->Val = Val;
    return Nd;
}

// 解析表达式
static Node *expr(Token **Rest, Token *Tok) { return equality(Rest, Tok); }

// 解析相等性

static Node *equality(Token **Rest, Token *Tok){
  // relational
  Node *Nd = relational(&Tok, Tok);

  // ("==" relational | "!=" relational)*
  while (true){
    // "==" relational
    if(equal(Tok, "==")){
      Nd = newBinary(ND_EQ, Nd, relational(&Tok, Tok->Next));
      continue;
    }

    // "!=" relational
    if(equal(Tok, "!=")){
      Nd = newBinary(ND_NE, Nd, relational(&Tok, Tok->Next));
      continue;
    }

    *Rest = Tok;
    return Nd;
  }
}

// 解析比较关系
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
static Node *relational(Token **Rest, Token *Tok){
  // add
  Node *Nd = add(&Tok, Tok);

  // ("<" add | "<=" add | ">" add | ">=" add)*
  while (true){
    // "<" add
    if (equal(Tok, "<")) {
      Nd = newBinary(ND_LT, Nd, add(&Tok, Tok->Next));
      continue;
    }

    // "<=" add
    if (equal(Tok, "<=")) {
      Nd = newBinary(ND_LE, Nd, add(&Tok, Tok->Next));
      continue;
    }

    // ">" add
    // X>Y等价于Y<X
    if (equal(Tok, ">")) {
      Nd = newBinary(ND_LT, add(&Tok, Tok->Next), Nd);
      continue;
    }

    // ">=" add
    // X>=Y等价于Y<=X
    if (equal(Tok, ">=")) {
      Nd = newBinary(ND_LE, add(&Tok, Tok->Next), Nd);
      continue;
    }

    *Rest = Tok;
    return Nd;
  }
}

// 解析加减

static Node *add(Token **Rest, Token *Tok)
{
  Node *Nd = mul(&Tok, Tok);

  // ("+" mul | "-" mul)*
  while (true) {
    // "+" mul
    if (equal(Tok, "+")) {
      Nd = newBinary(ND_ADD, Nd, mul(&Tok, Tok->Next));
      continue;
    }

    // "-" mul
    if (equal(Tok, "-")) {
      Nd = newBinary(ND_SUB, Nd, mul(&Tok, Tok->Next));
      continue;
    }

    *Rest = Tok;
    return Nd;
  }
}

// 解析乘除

static Node *mul(Token **Rest, Token *Tok) {
  // unary
  Node *Nd = unary(&Tok, Tok);

  // ("*" unary | "/" unary)*
  while (true) {
    // "*" unary
    if (equal(Tok, "*")) {
      Nd = newBinary(ND_MUL, Nd, unary(&Tok, Tok->Next));
      continue;
    }

    // "/" unary
    if (equal(Tok, "/")) {
      Nd = newBinary(ND_DIV, Nd, unary(&Tok, Tok->Next));
      continue;
    }

    *Rest = Tok;
    return Nd;
  }
}

// 解析一元运算数

static Node *unary(Token **Rest, Token *Tok){
    // '+' unary
    if(equal(Tok, "+"))
        return unary(Rest, Tok->Next);
    
    // '-' unary
    if(equal(Tok, "-"))
        return newUnary(ND_NEG, unary(Rest, Tok->Next));
    
    // primary
    return primary(Rest, Tok);
}

// 解析括号、数字

static Node *primary(Token **Rest, Token *Tok) {
  // "(" expr ")"
  if (equal(Tok, "(")) {
    Node *Nd = expr(&Tok, Tok->Next);
    *Rest = skip(Tok, ")");
    return Nd;
  }

  // num
  if (Tok->Kind == TK_NUM) {
    Node *Nd = newNum(Tok->Val);
    *Rest = Tok->Next;
    return Nd;
  }

  errorTok(Tok, "expected an expression");
  return NULL;
}

// 语法解析入口函数
Node *parse(Token *Tok) {
  Node *Nd = expr(&Tok, Tok);
  if (Tok->Kind != TK_EOF)
    errorTok(Tok, "extra token");
  return Nd;
}
