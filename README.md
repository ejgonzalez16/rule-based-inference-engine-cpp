# Predicate Logic Inference Engine in C++

A complete **logical inference engine** implemented from scratch in C++, capable of reading rule sets from `.txt` files, normalizing them, and performing **resolution-based inference** over first-order predicate logic expressions.

This project showcases:
- Manual parsing of logical rules  
- Universal (`*X*` → ∀) and existential (`@X@` → ∃) quantifier elimination  
- Skolemization  
- Predicate normalization  
- Handling of implications, biconditionals, conjunctions, disjunctions and negation  
- Unification of predicates with variables and constants  
- Resolution-based proof (returns *true* or *false* for the queried hypothesis)  
- Memory-safe, pointer-based implementation in modern C++

---

## 🚀 Features

### **✔ Rule Parsing and Normalization**
- Reads input knowledge-base rules from text files such as:
Pompeyano(marco)
XPompeyano(X)>Romano(X)
Gobernante(cesar)
XRomano(X)>[Leal(X,cesar)|Odia(X,cesar)]
X**Y[Hombre(X)&Gobernante(Y)&IntentaAsesinar(X,Y)]>-Leal(X,Y)
IntentaAsesinar(marco,cesar)

- Expands quantified variables  
- Replaces symbols (`>` → implication, `|` → OR, `&` → AND)  
- Skolemizes existential variables  
- Fully normalizes expressions into a resolution-friendly structure.

### **✔ First-Order Logic Support**
Fully supports:
- Unary and binary predicates
- Negation of simple and composite expressions
- Parenthesized expressions with nested logical operators
- Biconditional elimination (`A <-> B`)
- Implication elimination (`A -> B`)
- Logical distributive transformations

### **✔ Unification Engine**
- Matches predicates based on:
- Predicate name
- Number of arguments
- Variable/constant compatibility
- Builds substitution maps (variable → value)
- Propagates substitutions through all predicates

### **✔ Resolution-Based Inference**
After loading and normalizing the rule base, the engine:
- Reads a question from another `.txt` file  
- Negates the hypothesis  
- Attempts to derive a contradiction using resolution  
- Prints whether the conclusion is **true** or **false**

---

## 📁 Project Structure

Key components:

| File | Description |
|------|-------------|
| `Axioma.h / .cpp` | Represents normalized rules and collections of predicate facts. |
| `Afirmacion.h / .cpp` | Represents a single predicate instance with variables or constants. |
| `ValorPredicado.h` | Represents the value of each predicate argument. |
| `main.cpp` | Orchestrates file reading, normalization, unification, and resolution. |

The system processes two input files:

- **AxiomasPrueba.txt** – knowledge base
- **PreguntaPrueba.txt** – query to evaluate

---

