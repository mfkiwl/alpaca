#include <alpaca/alpaca.h>
#include <doctest.hpp>
using namespace alpaca;

using doctest::test_suite;

template <class T> struct Node {
  T data;
  std::unique_ptr<Node<T>> left;
  std::unique_ptr<Node<T>> right;

  Node(const T &data = T(), std::unique_ptr<Node<T>> lhs = nullptr,
       std::unique_ptr<Node<T>> rhs = nullptr)
      : data(data), left(std::move(lhs)), right(std::move(rhs)) {}

  Node(const Node &n) {
    data = n.data;
    left = n.left ? std::unique_ptr<Node<T>>{new Node<T>{*n.left}} : nullptr;
    right = n.right ? std::unique_ptr<Node<T>>{new Node<T>{*n.right}} : nullptr;
  }
};

template <class T>
auto make_node(T const &value, std::unique_ptr<Node<T>> lhs = nullptr,
               std::unique_ptr<Node<T>> rhs = nullptr) {
  return std::unique_ptr<Node<T>>(
      new Node<T>{value, std::move(lhs), std::move(rhs)});
}

TEST_CASE("Serialize unique_ptr<struct>" * test_suite("unique_ptr")) {

  std::vector<uint8_t> bytes;

  // serialize
  {
    /*
            5
           / \
          3   4
         / \
        1   2
    */

    auto const root =
        make_node(5, make_node(3, make_node(1), make_node(2)), make_node(4));

    bytes = serialize<Node<int>>(*root); // 15 bytes
  }

  {
    auto tree = deserialize<Node<int>>(bytes);
    REQUIRE(tree.data == 5);
    auto &left_subtree_0 = *(tree.left);
    REQUIRE(left_subtree_0.data == 3);
    const auto &right_subtree_0 = *(tree.right);
    REQUIRE(right_subtree_0.data == 4);
    REQUIRE(right_subtree_0.left == nullptr);
    REQUIRE(right_subtree_0.right == nullptr);
    const auto &left_subtree_1 = *(left_subtree_0.left);
    REQUIRE(left_subtree_1.data == 1);
    REQUIRE(left_subtree_1.left == nullptr);
    REQUIRE(left_subtree_1.right == nullptr);
    const auto &right_subtree_1 = *(left_subtree_0.right);
    REQUIRE(right_subtree_1.data == 2);
    REQUIRE(right_subtree_1.left == nullptr);
    REQUIRE(right_subtree_1.right == nullptr);
  }
}