#include <bits/stdc++.h>
using namespace std;

//表达式树节点
struct tree {
    char val;
    tree* left;
    tree* right;
};

//原子命题
struct atom {
    char val;
    bool neg=false;

    //重载set使用到的的比较运算符
    bool operator<(const atom& other) const {
        if (val != other.val) {
            return val<other.val;
        }
        return neg>other.neg;
    }
};

//操作优先级
int precedence(char c) {
    if(c=='!') return 4;
    if(c=='^') return 3;
    if(c=='v') return 2;
    if(c=='>') return 1;
    if(c=='-') return 1;
    if(c=='<') return 0;
    return -1;
}

//打印二叉树
void printTree(tree* root,int depth=0) {
    if(root==nullptr) return;
    printTree(root->right,depth+1);
    for (int i=0;i<depth;++i) cout<<"  ";
    cout<<root->val<<endl;
    printTree(root->left,depth+1);
}

//打印表达式
void printExpression(tree* root,int depth=0) {
    if(root==nullptr) return;
    if((root->left!=nullptr||root->val=='!')&&depth!=0) {
        cout<<"(";
    }
    printExpression(root->left,depth+1);
    if(root->val=='>') {
        cout<<"->";
    } else if(root->val=='<') {
        cout<<"<->";
    } else {
        cout<<root->val;
    }
    printExpression(root->right,depth+1);
    if(root->right!=nullptr&&depth!=0) {
        cout<<")";
    }
}

//读取表达式
tree* read(string line,bool& notEmptyReturn,bool& emptyReturn) {
    if(line.empty()) {
        emptyReturn=true;
        return nullptr;
    } else {
        notEmptyReturn=true;
    }

    stack<tree*> subTreeStack;
    stack<char> operatorStack;

    line.erase(remove_if(line.begin(),line.end(),[](char c) {
        return !(isalpha(c)||c=='^'||c=='!'||c=='-'||c=='<'||c=='>'||c=='('||c==')');
    }),line.end());

    for(int i=0;i<line.length();++i) {
        if(isalpha(line[i])&&line[i]!='v') {
            tree* node=new tree;
            node->val=line[i];
            node->left=nullptr;
            node->right=nullptr;
            subTreeStack.push(node);
        } else if(line[i]=='(') {
            operatorStack.push('(');
        } else if(line[i]==')') {
            if(operatorStack.empty()) {
                return nullptr;
            }
            while(!operatorStack.empty()&&operatorStack.top()!='(') {
                if(subTreeStack.empty()) {
                    return nullptr;
                }
                tree* node=new tree;
                node->val=operatorStack.top();
                node->right=subTreeStack.top();
                subTreeStack.pop();
                if(operatorStack.top()!='!') {
                    if(subTreeStack.empty()) {
                        return nullptr;
                    }
                    node->left=subTreeStack.top();
                    subTreeStack.pop();
                } else {
                    node->left=nullptr;
                }
                subTreeStack.push(node);
                operatorStack.pop();
            }
            if(!operatorStack.empty()) {
                operatorStack.pop();
            } else {
                return nullptr;
            }
        } else if(line[i]=='!') {
            operatorStack.push('!');
        } else if(line[i]=='v') {
            while(!operatorStack.empty()&&precedence(operatorStack.top())>=precedence(line[i])) {
                if(subTreeStack.empty()) {
                    return nullptr;
                }
                tree* node=new tree;
                node->val=operatorStack.top();
                node->right=subTreeStack.top();
                subTreeStack.pop();
                if(operatorStack.top()!='!') {
                    if(subTreeStack.empty()) {
                        return nullptr;
                    }
                    node->left=subTreeStack.top();
                    subTreeStack.pop();
                } else {
                    node->left=nullptr;
                }
                subTreeStack.push(node);
                operatorStack.pop();
            }
            operatorStack.push(line[i]);
        } else if(line[i]=='^') {
            while(!operatorStack.empty()&&precedence(operatorStack.top())>=precedence(line[i])) {
                if(subTreeStack.empty()) {
                    return nullptr;
                }
                tree* node=new tree;
                node->val=operatorStack.top();
                node->right=subTreeStack.top();
                subTreeStack.pop();
                if(operatorStack.top()!='!') {
                    if(subTreeStack.empty()) {
                        return nullptr;
                    }
                    node->left=subTreeStack.top();
                    subTreeStack.pop();
                } else {
                    node->left=nullptr;
                }
                subTreeStack.push(node);
                operatorStack.pop();
            }
            operatorStack.push(line[i]);
        } else if(line[i]=='-'&&line[i+1]=='>') {
            while(!operatorStack.empty()&&precedence(operatorStack.top())>=precedence(line[i])) {
                if(subTreeStack.empty()) {
                    return nullptr;
                }
                tree* node=new tree;
                node->val=operatorStack.top();
                node->right=subTreeStack.top();
                subTreeStack.pop();
                if(operatorStack.top()!='!') {
                    if(subTreeStack.empty()) {
                        return nullptr;
                    }
                    node->left=subTreeStack.top();
                    subTreeStack.pop();
                } else {
                    node->left=nullptr;
                }
                subTreeStack.push(node);
                operatorStack.pop();
            }
            ++i;
            operatorStack.push('>');
        } else if(line[i]=='<'&&line[i+1]=='-'&&line[i+2]=='>') {
            while(!operatorStack.empty()&&precedence(operatorStack.top())>precedence(line[i])) {
                if(subTreeStack.empty()) {
                    return nullptr;
                }
                tree* node=new tree;
                node->val=operatorStack.top();
                node->right=subTreeStack.top();
                subTreeStack.pop();
                if(operatorStack.top()!='!') {
                    if(subTreeStack.empty()) {
                        return nullptr;
                    }
                    node->left=subTreeStack.top();
                    subTreeStack.pop();
                } else {
                    node->left=nullptr;
                }
                subTreeStack.push(node);
                operatorStack.pop();
            }
            i+=2;
            operatorStack.push('<');
        } else {
            return nullptr;
        }
    }

    while(!operatorStack.empty()) {
        if(subTreeStack.empty()) {
            return nullptr;
        }
        if(operatorStack.top()=='(') {
            return nullptr;
        }
        tree* node=new tree;
        node->val=operatorStack.top();
        node->right=subTreeStack.top();
        subTreeStack.pop();
        if(operatorStack.top()!='!') {
            if(subTreeStack.empty()) {
                return nullptr;
            }
            node->left=subTreeStack.top();
            subTreeStack.pop();
        } else {
            node->left=nullptr;
        }
        subTreeStack.push(node);
        operatorStack.pop();
    }

    if(subTreeStack.size()>1) {
        return nullptr;
    }
    return subTreeStack.top();
}

//消除蕴含连接词
tree* elimImp(tree* treenode) {
    if(treenode->val=='>') {
        treenode->val='v';
        tree* temp=new tree;
        temp->val='!';
        temp->right=treenode->left;
        treenode->left=temp;
    } else if(treenode->val=='<') {
        treenode->val='^';
        tree* temp1=new tree;
        tree* temp2=new tree;
        temp1->val='>';
        temp1->left=treenode->left;
        temp1->right=treenode->right;
        temp2->val='>';
        temp2->left=treenode->right;
        temp2->right=treenode->left;
        treenode->left=temp1;
        treenode->right=temp2;
    }
    if(treenode->left!=nullptr) {
        treenode->left=elimImp(treenode->left);
    }
    if(treenode->right!=nullptr) {
        treenode->right=elimImp(treenode->right);
    }
    return treenode;
}

//否定词内移
tree* pushNeg(tree* treenode) {
    if(treenode->val=='!') {
        if(treenode->right->val=='!') {
            treenode->val=treenode->right->right->val;
            treenode->left=treenode->right->right->left;
            treenode->right=treenode->right->right->right;
        } else if(treenode->right->val=='v') {
            treenode->val='^';
            treenode->right->val='!';
            tree* temp=new tree;
            temp->val='!';
            temp->left=nullptr;
            temp->right=treenode->right->left;
            treenode->left=temp;
            treenode->right->left=nullptr;
        } else if(treenode->right->val=='^') {
            treenode->val='v';
            treenode->right->val='!';
            tree* temp=new tree;
            temp->val='!';
            temp->left=nullptr;
            temp->right=treenode->right->left;
            treenode->left=temp;
            treenode->right->left=nullptr;
        }
    }

    /* if(treenode->right!=nullptr&&treenode->right->right!=nullptr) {
        printExpression(treenode->right->right);
        cout<<endl;
    } */

    if(treenode->left!=nullptr) {
        treenode->left=pushNeg(treenode->left);
    }

    /* if(treenode->right!=nullptr&&treenode->right->right!=nullptr) {
        printExpression(treenode->right->right);
        cout<<endl;
    } */

    if(treenode->right!=nullptr) {
        treenode->right=pushNeg(treenode->right);
    }

    return treenode;
}

//利用分配律化简为合取范式
tree* distriLaw(tree* treenode) {
    if(treenode->val=='v') {
        if(treenode->left!=nullptr) {
            if(treenode->left->val=='^') {
                treenode->val='^';
                tree* temp1=new tree;
                tree* temp2=new tree;
                temp1->val='v';
                temp1->left=treenode->left->left;
                temp1->right=treenode->right;
                temp2->val='v';
                temp2->left=treenode->left->right;
                temp2->right=treenode->right;
                treenode->left=temp1;
                treenode->right=temp2;
            }
        }
        if(treenode->right!=nullptr) {
            if(treenode->right->val=='^') {
                treenode->val='^';
                tree* temp1=new tree;
                tree* temp2=new tree;
                temp1->val='v';
                temp1->left=treenode->left;
                temp1->right=treenode->right->left;
                temp2->val='v';
                temp2->left=treenode->left;
                temp2->right=treenode->right->right;
                treenode->left=temp1;
                treenode->right=temp2;
            }
        }
    }
    if(treenode->left!=nullptr) {
        treenode->left=distriLaw(treenode->left);
    }
    if(treenode->right!=nullptr) {
        treenode->right=distriLaw(treenode->right);
    }
    return treenode;
}

//化简表达式
tree* simplify(tree* treenode) {
    return distriLaw(pushNeg(elimImp(treenode)));
}

//提取字句集
void constructClauses(tree* root,vector<set<atom>>& clauses) {
    if(root==nullptr) {
        return;
    }
    if(root->val=='^') {
        constructClauses(root->left,clauses);
        constructClauses(root->right,clauses);
    } else if(root->val=='v') {
        set<atom> clause;
        vector<set<atom>> leftclauses;
        vector<set<atom>> rightclauses;
        constructClauses(root->left,leftclauses);
        constructClauses(root->right,rightclauses);
        if(!leftclauses.empty()) clause.insert(leftclauses[0].begin(), leftclauses[0].end());
        if(!rightclauses.empty()) clause.insert(rightclauses[0].begin(), rightclauses[0].end());
        clauses.push_back(clause);
    } else {
        set<atom> clause;
        atom tempatom;
        if(root->val=='!') {
            tempatom.neg=true;
            tempatom.val=root->right->val;
        } else {
            tempatom.val=root->val;
        }
        clause.insert(tempatom);
        clauses.push_back(clause);
    }
}

//打印字句集
void printClauses(vector<set<atom>>& clauses) {
    for(int i=0;i<clauses.size();i++) {
        cout<<"{";
        for(auto it=clauses[i].begin();it!=clauses[i].end();++it) {
            if(it!=clauses[i].begin()&&it!=clauses[i].end()) {
                cout<<",";
            }
            if(it->neg) {
                cout<<"!";
            }
            cout<<it->val;
        } cout<<"}"<<endl;
    }
}

//进行归结
bool ifCanResolve(set<atom>& clause1,set<atom>& clause2,atom& find) {
    for(auto it=clause1.begin();it!=clause1.end();++it) {
        atom negIt;
        negIt.neg=(find.neg ? false : true);
        negIt.val=it->val;
        if(clause2.find(negIt)!=clause2.end()) {
            find=*it;
            return true;
        }
    }
    return false;
}

set<atom> resolve(set<atom>& clause1,set<atom>& clause2,atom& find) {
    set<atom> result=clause1;
    result.erase(find);
    for(auto it=clause2.begin();it!=clause2.end();++it) {
        if(find.neg) {
            if(!(!it->neg&&it->val==find.val)) {
                result.insert(*it);
            }
        } else {
            if(!(it->neg&&it->val==find.val)) {
                result.insert(*it);
            }
        }
    }
    return result;
}

bool resoalgorithm(vector<set<atom>>& clauses) {
    set<set<atom>> beenInVector;
    while(true) {
        bool newClauseFound=false;
        int iniSize=clauses.size();
        for(int i=0;i<clauses.size();++i) {
            for(int j=0;j<clauses.size();++j) {
                if(j==i) continue;
                atom find;
                if(ifCanResolve(clauses[i],clauses[j],find)) {
                    set<atom> newClause=resolve(clauses[i],clauses[j],find);
                    if (newClause.empty()) {
                        return true;
                    }
                    if (beenInVector.find(newClause)==beenInVector.end()) {
                        clauses.push_back(newClause);
                        beenInVector.insert(newClause);
                        newClauseFound=true;
                    }
                }
            }
        }
        if (clauses.size()==iniSize) {
            return false;
        }
    }
}

int main() {
    cout<<"Please enter precondition:";
    tree* preconditionInput=nullptr;
    bool notEmptyReturn=false;
    while(true) {
        string line;
        getline(cin,line);
        if(line=="END") {
            break;
        } else {
            if(preconditionInput==nullptr) {
                bool emptyReturn=false;
                preconditionInput=read(line,notEmptyReturn,emptyReturn);
                if(preconditionInput==nullptr&&!emptyReturn) {
                    cout<<"Empty Input or Not Well-Formed Formula";
                    return 0;
                }
            } else {
                tree* temp=new tree;
                temp->val='^';
                temp->left=preconditionInput;
                bool emptyReturn=false;
                temp->right=read(line,notEmptyReturn,emptyReturn);
                if(temp->right!=nullptr) {
                    preconditionInput=temp;
                } else {
                    if(!emptyReturn) {
                        cout<<"Empty Input or Not Well-Formed Formula";
                        return 0;
                    } else {
                        delete temp;
                    }
                }
            }
        }
        cout<<"Continue or END:";
    }
    if(notEmptyReturn) {
        cout<<"Preconditon Constructed:"<<endl;
        printExpression(preconditionInput); cout<<endl;
    }

    cout<<"Please enter conclusion:";
    tree* conclusionInput=nullptr;
    string line;
    getline(cin,line);

    bool meaninglessEmptyReturn=false;
    conclusionInput=read(line,notEmptyReturn,meaninglessEmptyReturn);
    if(conclusionInput==nullptr) {
        cout<<"Empty Input or Not Well-Formed Formula";
        return 0;
    }

    cout<<"Conclusion Constructed:"<<endl;
    printExpression(conclusionInput); cout<<endl;

    tree* resoProp=new tree;
    if(preconditionInput==nullptr) {
        tree* tempRight=new tree;
        tempRight->val='!';
        tempRight->left=nullptr;
        tempRight->right=conclusionInput;
        resoProp=tempRight;
    } else {
        resoProp->val='^';
        resoProp->left=preconditionInput;
        tree* tempRight=new tree;
        tempRight->val='!';
        tempRight->left=nullptr;
        tempRight->right=conclusionInput;
        resoProp->right=tempRight;
    }


    resoProp=elimImp(resoProp);
    vector<set<atom>> clauses;
    resoProp=simplify(resoProp);
    cout<<"Proposition Used in Revolution:"<<endl;
    printExpression(resoProp); cout<<endl;
    cout<<"Simplified Version:"<<endl;
    printExpression(resoProp); cout<<endl;
    constructClauses(resoProp,clauses);
    cout<<"Clauses Constructed:"<<endl;
    printClauses(clauses);

    cout<<(resoalgorithm(clauses) ? "Empty clause found. It's true!" : "Resolution failed. Can't be proved.");
}