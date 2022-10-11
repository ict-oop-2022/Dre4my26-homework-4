#include "phone-book.h"
#include <algorithm>


bool phone_book_t::create_user(const std::string &number, const std::string &name) {
  for (auto & _user : _users) {
    if (_user.number == number) {
      return false;
    }
  }

  user_t user = {number, name};
  _users.push_back(user);

  user_info_t user_info = {user, 0};
  _userInfo.push_back(user_info);

  return true;
}

bool phone_book_t::add_call(const call_t &call) {
  for (auto & i : _userInfo) {
    if (i.user.number == call.number) {
      _calls.push_back(call);
      i.total_call_duration_s += call.duration_s;
      return true;
    }
  }

  return false;
}

std::vector<call_t> phone_book_t::get_calls(size_t start_pos, size_t count) const {

  if (start_pos > _calls.size()) {
    start_pos = _calls.size();
  }

  auto end = start_pos + count;

  if (_calls.size() < end) {
    end = _calls.size();
  }

  std::vector<call_t> result(end - start_pos);

  if (end != start_pos) {
    std::copy(_calls.begin() + start_pos, _calls.begin() + end, result.begin());
  }

  return result;
}

bool isPrefix(std::string_view prefix, std::string_view full)
{
  return prefix == full.substr(0, prefix.size());
}

bool comp_number ( const user_info_t& a,  const user_info_t& b)
{
  if(a.total_call_duration_s != b.total_call_duration_s)
    return a.total_call_duration_s > b.total_call_duration_s;
  if(a.user.name != b.user.name)
    return a.user.name < b.user.name;
  return a.user.number < b.user.number;
}

std::vector<user_info_t> phone_book_t::search_users_by_number(const std::string &number_prefix, size_t count) const {
  std::vector<user_info_t> result;

  for (auto &i: _userInfo) {

    if (isPrefix(number_prefix, i.user.number)) {
      result.push_back(i);
    }
  }

  std::sort(result.begin(), result.end(), comp_number);

  return std::vector<user_info_t>(result.begin(), result.begin() + std::min<size_t>(count, result.size()));
}

bool comp_name ( const user_info_t& a,  const user_info_t& b)
{
  if(a.user.name != b.user.name)
    return a.user.name < b.user.name;
  if(a.total_call_duration_s != b.total_call_duration_s)
    return a.total_call_duration_s > b.total_call_duration_s;

  return a.user.number < b.user.number;
}

std::vector<user_info_t> phone_book_t::search_users_by_name(const std::string &name_prefix, size_t count) const {
  std::vector<user_info_t> result;

  for (auto &i: _userInfo) {

    if (isPrefix(name_prefix, i.user.name)) {
      result.push_back(i);
    }
  }

  std::sort(result.begin(), result.end(), comp_name);

  return std::vector<user_info_t>(result.begin(), result.begin() + std::min<size_t>(count, result.size()));
}


void phone_book_t::clear() {
  _users.clear();
  _calls.clear();
  _userInfo.clear();
}

size_t phone_book_t::size() const {
  return _users.size();
}

bool phone_book_t::empty() const {
  return _users.empty();
}