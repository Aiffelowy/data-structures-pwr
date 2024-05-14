#ifndef PARSER_H
#define PARSER_H


#include "menu.hpp"
#include "../error/result.hpp"
#include "../error/option.hpp"
#include "../misc/misc.hpp"
#include <memory>
#include <string>
#include <vector>


namespace MenuN::Parser {

enum class CfgError {
  Ok,
  NoId,
  UnknownKeyword
};


struct CA {
  std::string name;
  std::vector<std::string> args;
  std::string so_file = "";
  
  constexpr CA() {}
  constexpr CA(std::string name) : name(name) {}

  constexpr int get_arg_size() const {
    return args.size();
  }
};


struct CaCtx {
  std::vector<CA> avail_types;

  Option::Option<CA&> get_type(std::string s) {
    for(auto& type : avail_types) {
      if(type.name == s) return Option::Some(&type);
    }
    return Option::None();
  }
};

struct MnCtx {
  std::string id = "";
  std::string title = "";
  std::string subtitle = "";

  std::vector<std::shared_ptr<Actions::Action>> actions;

  Result::Result<Submenu::SubMenu, CfgError> into_submenu() {
    if(id == "") return Result::Err(CfgError::NoId);
    Submenu::SubMenu temp(id);
    temp.set_title(title)
      .set_subtitle(subtitle);
    
    for(auto action : actions) {
      Misc::println("{}", action->get_label());
      temp.add_action(action);
    }

    return Result::Ok(temp);
  }
};

struct Parser {
private:
  std::string filename;

public:
  Parser(std::string);
  std::string remove_spaces(std::string);
  std::string remove_spaces_qouted(std::string);
  Result::Result<Menu, CfgError> to_menu();

};

}

#endif
