
*ПРАВКИ:*

  Add separate namespace for all user-defined classes          (DONE)

  std::string -> const &std::string                            (DONE)

*Simple class:*                                                (DONE)

  fix: * works as *=                                           (DONE)

*Complex class:*

  REWRITE own stl (vector was a bad choice; maybe set or map)  ()

  safe copy-assignment - no premature data deletion            (DONE)

  check for access beyond array bounds in vec operator[]       (DONE)
  #  it was used only for tests LOL

  return enum as state                                         (DONE)

  check what is called in MoveConstructor                      (DONE)
  #  vec move-c wasn't called from table mov-c, now it is


NOTE: pleeeeaase, write docs on private fields/classes
