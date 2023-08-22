; namespace
(def *ns*)
(def in-ns)

; miscellaneous
(def true?
  (fn* [o]
    (native/raw "__value = #{ o }#->equal(*#{ true }#) ? #{ true }# : #{ false }#")))
(def false?
  (fn* [o]
    (native/raw "__value = #{ o }#->equal(*#{ false }#) ? #{ true }# : #{ false }#")))
(def not
  (fn* [o]
    (native/raw "__value = #{ o }#->equal(*#{ false }#) ? #{ true }# : #{ false }#")))
(def nil?
  (fn* [o]
    (native/raw "__value = (o == OMI_NIL) ? #{ true }# : #{ false }#")))
(def some?
  (fn* [o]
    (native/raw "__value = (o == OMI_NIL) ? #{ false }# : #{ true }#")))