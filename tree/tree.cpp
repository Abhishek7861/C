#include<bits/stdc++.h>
using namespace std;

struct Node
{
    int data;
    Node *left, *right;
};


void inorder(Node* root)
{
    if(root!= NULL)
    {
        cout<<root->data<<" ";
        inorder(root->left);
        inorder(root->right);
    }
}


Node * newNode(int data)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = node->right= NULL;
    return (node);
}

Node* insertLevelOrder(int arr[], Node* root,int i,int n)
{
    if(i<n)
    {
        Node* temp = newNode(arr[i]);
        root = temp;
        root->left = insertLevelOrder(arr,root->left,2*i+1,n);
        root->right = insertLevelOrder(arr,root->right,2*i+2,n);
    }
    return root;
}


int maxdiff(Node *root)
{
    int n=0;
    if((root->right->right)==NULL)
    {
        n = max(abs(root->data-root->right->data),abs(root->data-root->left->data));
        return n;
    }  
    else
    {
      n = max(abs(root->data-root->left->data)+abs(maxdiff(root->left)),abs(root->data-root->right->data)+abs(maxdiff(root->right)));
        return n;
    }
}

int main()
{
    int arr[] = {55,21,33,43,55,23,78};
    int n = sizeof(arr)/sizeof(arr[0]);
    Node* root = insertLevelOrder(arr,root,0,n);
    // inorder(root);
    cout<< maxdiff(root)<<endl;
}