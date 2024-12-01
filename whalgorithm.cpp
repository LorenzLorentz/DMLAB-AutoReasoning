#include <bits/stdc++.h>
using namespace std;

//ofstream cout("falseProp3.log");

//定义二叉树节点
struct tree {
    char val;
    tree *left;
    tree *right;
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

//打印函数
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

//读取函数
tree* read() {
    //子树栈，操作栈
    stack<tree*> subTreeStack;
    stack<char> operatorStack;

    string line;
    getline(cin,line);
    if(line.empty()) return nullptr;

    line.erase(remove_if(line.begin(),line.end(),[](char c) {
        return !(isalpha(c)||c=='^'||c=='!'||c=='-'||c=='<'||c=='>'||c=='('||c==')');
    }),line.end());
    cout<<"Processed input: "<<line<<endl;

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

    cout<<"Binary Tree Constructed: "<<endl;
    printTree(subTreeStack.top());
    cout<<"Expression Constructed: "<<endl;
    printExpression(subTreeStack.top());
    cout<<endl;
    return subTreeStack.top();
}

//判断后键是否可以直接返回
bool backFrontCheck(vector<tree*> head) {
    for(int i=0;i<head.size();++i) {
        if(!isalpha(head[i]->val)||head[i]->val=='v') {
            return false;
        }
    }
    return true;
}
bool backBackCheck(vector<tree*> tail,int i) {
    for(int j=i+1;j<tail.size();++j) {
        if(!isalpha(tail[j]->val)||tail[j]->val=='v') {
            return false;
        }
    }
    return true;
}

//判断前键是否可以直接返回
bool frontFrontCheck(vector<tree*> head,int i) {
    for(int j=i+1;j<head.size();++j) {
        if(!isalpha(head[j]->val)||head[j]->val=='v') {
            return false;
        }
    }
    return true;
}

//王浩算法
bool whalgorithm(vector<tree*>& head,vector<tree*>& tail) {
    cout<<"*** ";
    for(int j=0;j<head.size();j++) {
        if(j!=0) cout<<",";
        printExpression(head[j]);
    } cout<<" => ";
    for(int j=0;j<tail.size();j++) {
        if(j!=0) cout<<",";
        printExpression(tail[j]);
    } cout<<endl;

    for(int i=0;i<tail.size();i++) {
        if(!isalpha(tail[i]->val)||tail[i]->val=='v') {
            char c=tail[i]->val;
            if(c=='!') {
                vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                temp1.push_back(tail[i]->right);
                temp2.erase(temp2.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(backFrontCheck(head)&&backBackCheck(tail,i)) {
                    return true;
                }
            } else if(c=='v') {
                vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                temp2.push_back(tail[i]->left);
                temp2.push_back(tail[i]->right);
                temp2.erase(temp2.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(backFrontCheck(head)&&backBackCheck(tail,i)) {
                    return true;
                }
            } else if(c=='^') {
                vector<tree*> temp1=head;
                vector<tree*> temp21=tail;
                vector<tree*> temp22=tail;
                temp21.push_back(tail[i]->left);
                temp21.erase(temp21.begin()+i);
                temp22.push_back(tail[i]->right);
                temp22.erase(temp22.begin()+i);
                if(!whalgorithm(temp1,temp21)||!whalgorithm(temp1,temp22)) {
                    return false;
                } else if(backFrontCheck(head)&&backBackCheck(tail,i)) {
                    return true;
                }
            } else if(c=='>') {
                /* vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                tree* tempnode=new tree;
                tree* templeft=new tree;
                tempnode->val='v';
                templeft->val='!';
                templeft->left=nullptr;
                templeft->right=new tree(*tail[i]->left);
                tempnode->left=new tree(*templeft);
                tempnode->right=new tree(*tail[i]->right);
                temp2.push_back(tempnode);
                temp2.erase(tail.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(backFrontCheck(head)&&backBackCheck(tail,i)) {
                    return true;
                } */
                vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                temp1.push_back(tail[i]->left);
                temp2.push_back(tail[i]->right);
                temp2.erase(temp2.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(backFrontCheck(head)&&backBackCheck(tail,i)) {
                    return true;
                }
            } else if(c=='<') {
                vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                temp1.push_back(tail[i]->left);
                temp2.push_back(tail[i]->right);
                temp2.erase(temp2.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                }
                temp1=head;
                temp2=tail;
                temp1.push_back(tail[i]->right);
                temp2.push_back(tail[i]->left);
                temp2.erase(temp2.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(backFrontCheck(head)&&backBackCheck(tail,i)) {
                    return true;
                }
            }
        }
    }

    for(int i=0;i<head.size();++i) {
        if(!isalpha(head[i]->val)||head[i]->val=='v') {
            char c=head[i]->val;
            if(c=='!') {
                vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                temp2.push_back(head[i]->right);
                temp1.erase(temp1.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(frontFrontCheck(head,i)) {
                    return true;
                }
            } else if(c=='^') {
                vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                temp1.push_back(head[i]->left);
                temp1.push_back(head[i]->right);
                temp1.erase(temp1.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(frontFrontCheck(head,i)) {
                    return true;
                }
            } else if(c=='v') {
                vector<tree*> temp11=head;
                vector<tree*> temp12=head;
                vector<tree*> temp2=tail;
                temp11.push_back(head[i]->left);
                temp12.push_back(head[i]->right);
                temp11.erase(temp11.begin()+i);
                temp12.erase(temp12.begin()+i);
                if(!whalgorithm(temp11,temp2)||!whalgorithm(temp12,temp2)) {
                    return false;
                } else if(frontFrontCheck(head,i)) {
                    return true;
                } else {
                }
            } else if(c=='>') {
                vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                temp1.push_back(head[i]->right);
                temp1.erase(temp1.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(frontFrontCheck(head,i)) {
                    return true;
                }

                temp1=head;
                temp2=tail;
                temp2.push_back(head[i]->left);
                temp1.erase(temp1.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(frontFrontCheck(head,i)) {
                    return true;
                }
            } else if(c=='<') {
                vector<tree*> temp1=head;
                vector<tree*> temp2=tail;
                temp1.push_back(head[i]->left);
                temp1.push_back(head[i]->right);
                temp1.erase(temp1.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(frontFrontCheck(head,i)) {
                    return true;
                }

                temp1=head;
                temp2=tail;
                temp2.push_back(head[i]->left);
                temp2.push_back(head[i]->right);
                temp1.erase(temp1.begin()+i);
                if(!whalgorithm(temp1,temp2)) {
                    return false;
                } else if(frontFrontCheck(head,i)) {
                    return true;
                }
            }
        }
    }

    string compare1;
    string compare2;
    for(int j=0;j<head.size();++j) {
        compare1+=head[j]->val;
    }
    for(int j=0;j<tail.size();++j) {
        compare2+=tail[j]->val;
    }
    sort(compare1.begin(),compare1.end());
    sort(compare2.begin(),compare2.end());
    vector<char> intersection;
    set_intersection(compare1.begin(), compare1.end(), compare2.begin(), compare2.end(), back_inserter(intersection));
    if(!intersection.empty()) {
        cout<<" --- ";
        cout<<compare1<<" | "<<compare2<<" ---";
        cout<<"TRUE"<<endl;
        return true;
    } else {
        cout<<" --- ";
        cout<<compare1<<" | "<<compare2<<" ---";
        cout<<"FALSE"<<endl;
        return false;
    }
}

int main() {
    vector<tree*> head;
    vector<tree*> tail;
    tree* inputExpression=read();
    if(inputExpression==nullptr) {
        cout<<"Empty Input or Not Well-Formed Formula";
    } else {
        tail.push_back(inputExpression);
        cout<<"Start Reasoning:"<<endl;
        cout<<(whalgorithm(head,tail) ? "It's true!" : "It's false.");
    }
}