#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

class Node : public std::enable_shared_from_this<Node>
{
public:
	using ptr = std::shared_ptr<Node>;


};