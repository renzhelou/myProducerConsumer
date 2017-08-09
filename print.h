// 递归打印
void showall() { std::cout<<std::endl; }

template <typename R1 ,typename...  Args>
void showall(R1 var, Args...args) {
    std::cout << var;
    showall(args...);
}
