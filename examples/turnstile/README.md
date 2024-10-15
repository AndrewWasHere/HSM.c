# Turnstile Example

The Turnstile example implements the following state machine:

```plantuml
@startuml
state locked
state unlocked {
  token inserted / return token
}

[*] --> locked
locked -> unlocked : token inserted / unlatch turnstile
unlocked -> locked : turnstile actuated / latch turnstile
@enduml
```