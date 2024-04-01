// /****************************************************************************************
// *  Ugonna Ezeokoli
// *  uezeokol
// *  2024 Winter CSE101 PA8
// *  Dictionary.cpp
// *  Implementation file for Dictionary ADT
// *****************************************************************************************/

#include<iostream>
#include<string>
#include<stdexcept>
#include"Dictionary.h"
#include<ctype.h>


#define RED 1
#define BLACK 0
// Class Constructors & Destructors ----------------------------------------

//Node constructor
Dictionary::Node::Node(keyType k, valType v){
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = BLACK;
}

// Creates new Dictionary in the empty state. 
Dictionary::Dictionary(){
    nil = new Node("0",0);
    root = nil;
    current = nil;
    num_pairs = 0;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D){
    nil =  new Node("0",0);
    root = nil;
    current = nil;
    num_pairs = 0;

    Node* R = D.root;
    this->preOrderCopy(R, D.nil);
}

// Destructor
Dictionary::~Dictionary(){
    this->clear();
    delete nil;
}


// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const{
    return num_pairs;
}

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const{
    Node* F = root; // Find
    while (F != nullptr){
        if ((k == F->key)){
            return true;
        }
        else if (k < F->key){
            F = F->left;
        }
        else{ // k > x.key
            F = F->right;
        }
    }
    return false;
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const{
    if (!this->contains(k)){
        throw std::invalid_argument("Dictionary getValue(): key does not exist");
    }
    Node* F = root; // Find
    while (F != nil){
        if ((k == F->key)){
            return F->val;
        }
        else if (k < F->key){
            F = F->left;
        }
        else{ // k > x.key
            F = F->right;
        }
    }
    return F->val;
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false 
// otherwise.
bool Dictionary::hasCurrent() const{
    return (current != nil);
}

// currentKey()
// Returns the current key.
// Pre: hasCurrent() 
keyType Dictionary::currentKey() const{
    if (!this->hasCurrent()){
        throw std::invalid_argument("Dictionary currentKey(): current is undefined");
    }
    
    return current->key;

}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{
    if (!this->hasCurrent()){
        throw std::invalid_argument("Dictionary currentVal(): current is undefined");
    }

    return current->val;
}


// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear(){
    while (num_pairs > 0){
        this->remove(root->key);
    }

}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v){
    Node* T = root; //traverse
    Node* P = nil; //parent
    while(T != nil){
        P = T;
        int comp =(k.compare(T->key));

        if (comp == 0){
            T->val = v;
            return;
        }
        else if(comp < 0){
            T = T->left;
        }
        else{
            T = T->right;
        }
    }
    //exits while loop means new pair added
    Node* I = new Node(k,v); // insert
    num_pairs++;
    I->parent = P;
    if (P == nil){ //tree empty
         root = I;
    }
    else if (I->key < P->key){
        P->left = I;
    }
    else{
        P->right = I;
    }
    I->left = nil;
    I->right = nil;
    I->color = RED;
    this->RB_InsertFixUp(I);
    //PSUEDO CODE
//    y = T.nil
//    x = T.root
//    while x != T.nil
//       y = x
//       if z.key < x.key
//          x = x.left
//       else 
//          x = x.right
//    z.parent = y
//    if y == T.nil
//       T.root = z
//    else if z.key < y.key
//       y.left = z
//    else 
//       y.right = z
//    z.left = T.nil
//    z.right = T.nil
//    z.color = RED
//    RB_InsertFixUp(T, z)
}

void Dictionary::RB_InsertFixUp(Dictionary::Node *Z){
    Node* Y;
    if (Z->parent == nil || Z->parent->parent == nil){
        root->color = BLACK;
        return;
    }
    while(Z->parent != nil && Z->parent->color == RED){
        if(Z->parent == Z->parent->parent->left){
            Y = nil;
            if(Z->parent->parent->right != nil){
                Y = Z->parent->parent->right;
            }   
            if(Y != nil && Y->color == RED){
                Z->parent->color = BLACK;
                Y->color = BLACK;
                Z->parent->parent->color = RED;
                Z = Z->parent->parent;
            }
            else{
                if(Z == Z->parent->right){
                    Z = Z->parent;
                    this->LeftRotate(Z);
                }
                Z->parent->color = BLACK;
                Z->parent->parent->color = RED;
                RightRotate(Z->parent->parent);
            }
        }
        else{
            Y = Z->parent->parent->left;
            if (Y != nil && Y->color == RED){
                Z->parent->color = BLACK;
                Y->color = BLACK;
                Z->parent->parent->color = RED;
                Z = Z->parent->parent;
                if (Z->parent == nil){
                    break;
                }
            }
            else{
                if(Z == Z->parent->left){
                    Z = Z->parent;
                    this->RightRotate(Z);

                }
                Z->parent->color = BLACK;
                Z->parent->parent->color = RED;
                LeftRotate(Z->parent->parent);
            }
        }
        
    }
    root->color = BLACK;

// PSUEDO CODE
//      while z.parent.color == RED
//       if z.parent == z.parent.parent.left
//          y = z.parent.parent.right
//          if y.color == RED
//             z.parent.color = BLACK              // case 1
//             y.color = BLACK                     // case 1
//             z.parent.parent.color = RED         // case 1
//             z = z.parent.parent                 // case 1
//          else 
//             if z == z.parent.right
//                z = z.parent                     // case 2
//                LeftRotate(T, z)                 // case 2
//             z.parent.color = BLACK              // case 3
//             z.parent.parent.color = RED         // case 3
//             RightRotate(T, z.parent.parent)     // case 3
//       else 
//          y = z.parent.parent.left
//          if y.color == RED
//             z.parent.color = BLACK              // case 4
//             y.color = BLACK                     // case 4
//             z.parent.parent.color = RED         // case 4
//             z = z.parent.parent                 // case 4
//          else 
//             if z == z.parent.left
//                z = z.parent                     // case 5
//                RightRotate(T, z)                // case 5
//             z.parent.color = BLACK              // case 6
//             z.parent.parent.color = RED         // case 6
//             LeftRotate(T, z.parent.parent)      // case 6
//    T.root.color = BLACK

}

void Dictionary::LeftRotate(Dictionary::Node *X){
    //PSUEDO CODE
    // set y
//    y = x.right 
   
//    // turn y's left subtree into x's right subtree
//    x.right = y.left 
//    if y.left != T.nil     // not necessary if using sentinal nil node
//       y.left.parent = x
   
//    // link y's parent to x
//    y.parent = x.parent 
//    if x.parent == T.nil
//       T.root = y
//    else if x == x.parent.left
//       x.parent.left = y
//    else 
//       x.parent.right = y
   
//    // put x on y's left
//    y.left = x 
//    x.parent = y

    Node* Y;
    Y = X->right;

    X->right = Y->left;
    if (Y->left != nil){
        Y->left->parent = X;
    }

    Y->parent = X->parent;
    if(X->parent == nil){
        root = Y;
    }
    else if (X == X->parent->left){
        X->parent->left = Y;
    }
    else{
        X->parent->right = Y;
    }

    Y->left = X;
    X->parent = Y;
}   

void Dictionary::RightRotate(Dictionary::Node *X){
    //PSUEDO CODE
//     RotateRight(T, x)
//    // set y
//    y = x.left 
   
//    // turn y's right subtree into x's left subtree
//    x.left = y.right 
//    if y.right != T.nil  // not necessary if using sentinal nil node
//       y.right.parent = x
   
//    // link y's parent to x
//    y.parent = x.parent 
//    if x.parent == T.nil
//       T.root = y
//    else if x == x.parent.right
//       x.parent.right = y
//    else 
//       x.parent.left = y
   
//    // put x on y's right
//    y.right = x 
//    x.parent = y

    Node* Y;
    Y = X->left;

    X->left = Y->right;
    if (Y->right != nil){
        Y->right->parent = X;
    }

    Y->parent = X->parent;
    if(X->parent == nil){
        root = Y;
    }
    else if (X == X->parent->right){
        X->parent->right = Y;
    }
    else{
        X->parent->left = Y;
    }

    Y->right = X;
    X->parent = Y;
   
}

// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k){
    if (!this->contains(k)){
        throw std::invalid_argument("Dictionary remove(): key does not exist");
    }

    // if current is pair then current undefined
    if (current != nil){
        if (this->currentKey() == k){
            current = nil;
        }
    }

    if (num_pairs == 1){
        delete root;
        root = nil;
        current = nil;
        num_pairs = 0;
        return;
    }

    Node* F = root; // Find
    while (F != nil){
        if ((k == F->key)){
            this->Delete(F);
            delete F;
            num_pairs--;
            return;
        }
        else if (k < F->key){
            F = F->left;
        }
        else{ // k > x.key
            F = F->right;
        }
    }


}

void Dictionary::Transplant(Dictionary::Node *U, Dictionary::Node *V){
// Transplant(T, u, v)
    // if u.parent == NIL
    //     T.root = v
    // else if u == u.parent.left
    //     u.parent.left = v
    // else 
    //     u.parent.right = v
    // if v != NIL
    //     v.parent = u.parent

    if (U->parent == nil){
        root = V;
    }
    else if (U == U->parent->left){
        U->parent->left = V;
    }
    else{
        U->parent->right = V;
    }
    
    V->parent = U->parent;
}

void Dictionary::Delete(Dictionary::Node *Z){
//    PSUEDO CODE
//    RB_Delete(T, z)
//    y = z
//    y_original_color = y.color
//    if z.left == T.nil
//       x = z.right
//       RB_Transplant(T, z, z.right)
//    else if z.right == T.nil
//       x = z.left
//       RB_Transplant(T, z, z.left)
//    else 
//       y = TreeMinimum(z.right)
//       y_original_color = y.color
//       x = y.right
//       if y.parent == z
//          x.parent = y
//       else 
//          RB_Transplant(T, y, y.right)
//          y.right = z.right
//          y.right.parent = y
//       RB_Transplant(T, z, y)
//       y.left = z.left
//       y.left.parent = y
//       y.color = z.color
//    if y_original_color == BLACK
//       RB_DeleteFixUp(T, x)
    Node* X;
    Node* Y = Z;
    int y_og_color = Y->color;
    if (Z->left == nil){
        X = Z->right;
        this->Transplant(Z,Z->right);
    }
    else if (Z->right == nil){
        X = Z->left;
        this->Transplant(Z,Z->left);
    }
    else{
        Y = this->findMin(Z->right);
        y_og_color = Y->color;
        X = Y->right;
        if (Y->parent == Z){
            X->parent = Y;
        }
        else{
            this->Transplant(Y,Y->right);
            Y->right = Z->right;
            Y->right->parent = Y;
        }
        this->Transplant(Z,Y);
        Y->left = Z->left;
        Y->left->parent = Y;
        Y->color = Z->color;
    }

    if (y_og_color == BLACK){
        RB_DeleteFixUp(X);
    }
        

}

void Dictionary::RB_DeleteFixUp(Dictionary::Node *X){
//    PSUEDO CODE
//    RB_DeleteFixUp(T, x)
//    while x != T.root and x.color == BLACK
//       if x == x.parent.left
//          w = x.parent.right
//          if w.color == RED
//             w.color = BLACK                        // case 1
//             x.parent.color = RED                   // case 1
//             LeftRotate(T, x.parent)                // case 1
//             w = x.parent.right                     // case 1
//          if w.left.color == BLACK and w.right.color == BLACK
//             w.color = RED                          // case 2
//             x = x.parent                           // case 2
//          else 
//             if w.right.color == BLACK
//                w.left.color = BLACK                // case 3
//                w.color = RED                       // case 3
//                RightRotate(T, w)                   // case 3
//                w = x.parent.right                  // case 3
//             w.color = x.parent.color               // case 4
//             x.parent.color = BLACK                 // case 4
//             w.right.color = BLACK                  // case 4
//             LeftRotate(T, x.parent)                // case 4
//             x = T.root                             // case 4
//       else 
//          w = x.parent.left
//          if w.color == RED
//             w.color = BLACK                        // case 5
//             x.parent.color = RED                   // case 5
//             RightRotate(T, x.parent)               // case 5
//             w = x.parent.left                      // case 5
//          if w.right.color == BLACK and w.left.color == BLACK
//             w.color = RED                          // case 6
//             x = x.parent                           // case 6
//          else 
//             if w.left.color == BLACK
//                w.right.color = BLACK               // case 7
//                w.color = RED                       // case 7
//                LeftRotate(T, w)                    // case 7
//                w = x.parent.left                   // case 7
//             w.color = x.parent.color               // case 8
//             x.parent.color = BLACK                 // case 8
//             w.left.color = BLACK                   // case 8
//             RightRotate(T, x.parent)               // case 8
//             x = T.root                             // case 8
//    x.color = BLACK

    Node* W;
    while(X != root && X->color == BLACK){
        if(X == X->parent->left){
            W = X->parent->right;
            if(W->color == RED){
                W->color = BLACK;
                X->parent->color = RED;
                this->LeftRotate(X->parent);
                W = X->parent->right;
            }
            if (W->left->color == BLACK && W->right->color == BLACK){
                W->color = RED;
                X = X->parent;
                
            }  
            else{
                if(W->right->color == BLACK){
                    W->left->color = BLACK;
                    W->color = RED;
                    this->RightRotate(W);
                    W = X->parent->right;
                }
                W->color = X->parent->color;
                X->parent->color = BLACK;
                W->right->color = BLACK;
                this->LeftRotate(X->parent);
                X = root;       //real


            }
        }
        else{
            W = X->parent->left;
            if(W->color == RED){
                W->color = BLACK;
                X->parent->color = RED;
                this->RightRotate(X->parent);
                W = X->parent->left;
            }
            if (W->right->color == BLACK && W->left->color == BLACK ){
                W->color = RED;
                X = X->parent;       //real
            } 
            else{
                if(W->left->color == BLACK){
                    W->right->color = BLACK;
                    W->color = RED;
                    this->LeftRotate(W);
                    W = X->parent->left;
                }
                W->color = X->parent->color;
                X->parent->color = BLACK;
                W->left->color = BLACK;
                this->RightRotate(X->parent);
                X = root;       //real

            }
        }
    }
    X->color = BLACK;

}

// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::begin(){
    if (num_pairs == 0){
        return;
    }
 
    current = root;
    while(current->left != nil){
        current = current->left;
    }
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end(){
    if (num_pairs == 0){
        return;
    }

    current = root;
    while(current->right != nil){
        current = current->right;
    }

}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next(){
    if (!this->hasCurrent()){
        throw std::invalid_argument("Dictionary next(): current is undefined");
    }

    // TreeSuccessor(x)
    // if x.right != NIL                 // case 1
    //     return TreeMinimum(x.right)
    // y = x.parent                      // case 2
    // while y != NIL and x == y.right
    //     x = y
    //     y = y.parent
    // return y


    Node* dummy;
    if(current->right != nil){
        current = current->right;
        while(current->left != nil){
            current = current->left;
        }
    }
    else{
        dummy = current->parent;
        while((dummy != nil) && (current == dummy->right)){
            current = dummy;
            dummy = dummy->parent;
        }
        current = dummy;
    }
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev(){
    if (!this->hasCurrent()){
        throw std::invalid_argument("Dictionary prev(): current is undefined");
    }

    Node* dummy;
    if(current->left != nil){
        current = current->left;
        while(current->right != nil){
            current = current->right;
        }
    }
    else{
        dummy = current->parent;
        while((dummy != nil) && (current == dummy->left)){
            current = dummy;
            dummy = dummy->parent;
        }
        current = dummy;
    }
}


// Helper Functions (Optional) ---------------------------------------------

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Dictionary::Node* R) const{
    std::string str_val;
    if (R != nil){
        inOrderString(s,R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s,R->right);
    }
}

// preOrderString()
// Appends a string representation of the tree rooted at R to s. The appended
// string consists of keys only, separated by "\n", with the order determined
// by a pre-order tree walk.
void Dictionary::preOrderString(std::string& s, Dictionary::Node *R) const{
    std::string str_val;
    if (R != nil){
        if(R->color == RED){
            s += R->key + " (RED)\n";
        }
        else{
            s += R->key + "\n";
        }
        preOrderString(s,R->left);
        preOrderString(s,R->right);
    }
}

// preOrderCopy()
// Recursively inserts a deep copy of the subtree rooted at R into this 
// Dictionary. Recursion terminates at N.
void Dictionary::preOrderCopy(Dictionary::Node *R, Dictionary::Node *N ){
    if (R != N){
        this->setValue(R->key,R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node *Dictionary::findMin(Dictionary::Node *R){
// PSUEDO
// TreeMinimum(x) Pre: x != NIL
//    while x.left != NIL
//       x = x.left
//    return x
    while(R->left != nil){
        R = R->left;
    }
    return R;
}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node *Dictionary::findMax(Dictionary::Node *R){
// Pseudo
// TreeMaximum(x) Pre: x != NIL
//    while x.right != NIL
//       x = x.right
//    return x
    while(R->right != nil){
        R = R->right;
    }
    return R;

}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const{
    std::string s;
    this->inOrderString(s,root);
    return s;

}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. Consecutive
// keys are separated by newline "\n" characters. The key order is given
// by a pre-order tree walk.
std::string Dictionary::pre_string() const{
    std::string s;
    this->preOrderString(s,root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const{
    Dictionary Copy_T = *this;
    Dictionary Copy_D = D;
    if (this->num_pairs != D.num_pairs){
        return false;
    }
    Copy_T.begin();
    Copy_D.begin();
    while (Copy_T.hasCurrent()){
        if (Copy_T.currentKey() != Copy_D.currentKey()){
            return false;
        }

        if (Copy_T.currentVal() != Copy_D.currentVal()){
            return false;
        }
        Copy_T.next();
        Copy_D.next();
    }
    return true;

}


// Overloaded Operators ----------------------------------------------------

// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream << D.to_string();
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=( const Dictionary& D ){
    if (this != &D){
        Dictionary Copy_D = D;

        std::swap(nil,Copy_D.nil);
        std::swap(root,Copy_D.root);
        std::swap(current,Copy_D.current);
        std::swap(num_pairs,Copy_D.num_pairs);
    }
    return *this;
}