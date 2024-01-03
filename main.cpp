#include <iostream>
#include <coroutine>
#include <memory>

template<typename T>
struct MyFuture{
   std::shared_ptr<T> value;

   explicit MyFuture(std::shared_ptr<T> p) : value{std::move(p)}{
       std::cout<<"Результат состояния вернули из promise_type"<<'\n';
   }

   ~MyFuture(){
       std::cout<<"Уничтожили (но после записи в promise_type)"<<'\n';
   }

    T get() {
       std::cout<< "Получили готовый результат по указателю value"<<'\n';
        return *value;
    }
    struct promise_type{

      std::shared_ptr<T> ptr = std::make_shared<T>();

     MyFuture<T>get_return_object(){
         std::cout<<"Вернули Указатель на область памяти с результатом"<<'\n';
         return MyFuture<T>{ptr};
     }
     void return_value(T v){
         std::cout<<"Передали результат из Корутины на указатель в нашу общую област памяти"<<'\n';
          *ptr = v;
     }
     std::suspend_never initial_suspend(){
         std::cout<<"Не останавливаем Корутину при старте"<<'\n';
         return {};
     }
     std::suspend_never final_suspend() noexcept{
         std::cout<<"Не останавливаем Корутину при завершении"<<'\n';
         return {};
     }
      void unhandled_exception(){
         std::exit(1);
     }
    };
};

MyFuture<int>createFuture(){
    std::cout<<"Создали экземпляр Future"<<'\n';
    co_return 2020;
}

int main() {
std::cout<<'\n';
auto fut = createFuture();
auto res = fut.get();
std::cout << "res: " << res << '\n';
std::cout << '\n';
    return 0;
}
