FUNCTION (FILE_HAS_Q_OBJECT var file)
    FILE(STRINGS ${file} lines)

    FOREACH(sourceline in ${lines})
      IF(sourceline MATCHES "Q_OBJECT")
        LIST(APPEND ${moc_list} ${sourcefile})
        SET(${var} TRUE PARENT_SCOPE)
        RETURN()
      ENDIF(sourceline MATCHES "Q_OBJECT")
    ENDFOREACH(sourceline)

    SET(${var} FALSE PARENT_SCOPE)
ENDFUNCTION (FILE_HAS_Q_OBJECT)

FUNCTION (FILTER_Q_OBJECT moc_list nomoc_list)
  SET(${moc_list})
  SET(${nomoc_list})

  FOREACH(sourcefile IN ITEMS ${ARGN})
    SET(qobject_found FALSE)
    FILE_HAS_Q_OBJECT(qobject_found ${sourcefile})

    IF(NOT qobject_found)
      LIST(APPEND ${nomoc_list} ${sourcefile})
    ELSE(NOT qobject_found)
      LIST(APPEND ${moc_list} ${sourcefile})
    ENDIF(NOT qobject_found)

  ENDFOREACH(sourcefile)

  SET(${moc_list} ${${moc_list}} PARENT_SCOPE)
  SET(${nomoc_list} ${${nomoc_list}} PARENT_SCOPE)
ENDFUNCTION(FILTER_Q_OBJECT)

FUNCTION(QT4_WRAP_CPP_FILTERED wrapped)
  FILTER_Q_OBJECT(MOC_LIST X ${ARGN})
  QT4_WRAP_CPP(output ${MOC_LIST})
  SET(wrapped ${output} PARENT_SCOPE)
ENDFUNCTION(QT4_WRAP_CPP_FILTERED)
