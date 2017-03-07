// RUN: %check_clang_tidy %s modernize-use-bool-literals %t

bool IntToTrue = 1;
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: converting integer literal to bool, use bool literal instead [modernize-use-bool-literals]
// CHECK-FIXES: {{^}}bool IntToTrue = true;{{$}}

bool IntToFalse(0);
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: {{.*}}
// CHECK-FIXES: {{^}}bool IntToFalse(false);{{$}}

bool LongLongToTrue{0x1LL};
// CHECK-MESSAGES: :[[@LINE-1]]:21: warning: {{.*}}
// CHECK-FIXES: {{^}}bool LongLongToTrue{true};{{$}}

bool ExplicitCStyleIntToFalse = (bool)0;
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: {{.*}}
// CHECK-FIXES: {{^}}bool ExplicitCStyleIntToFalse = false;{{$}}

bool ExplicitFunctionalIntToFalse = bool(0);
// CHECK-MESSAGES: :[[@LINE-1]]:37: warning: {{.*}}
// CHECK-FIXES: {{^}}bool ExplicitFunctionalIntToFalse = false;{{$}}

bool ExplicitStaticIntToFalse = static_cast<bool>(0);
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: {{.*}}
// CHECK-FIXES: {{^}}bool ExplicitStaticIntToFalse = false;{{$}}

#define TRUE_MACRO 1
// CHECK-FIXES: {{^}}#define TRUE_MACRO 1{{$}}

bool MacroIntToTrue = TRUE_MACRO;
// CHECK-MESSAGES: :[[@LINE-1]]:23: warning: converting integer literal to bool, use bool literal instead [modernize-use-bool-literals]
// CHECK-FIXES: {{^}}bool MacroIntToTrue = TRUE_MACRO;{{$}}

#define FALSE_MACRO bool(0)
// CHECK-FIXES: {{^}}#define FALSE_MACRO bool(0){{$}}

bool TrueBool = true; // OK

bool FalseBool = bool(FALSE_MACRO);
// CHECK-MESSAGES: :[[@LINE-1]]:23: warning: {{.*}}
// CHECK-FIXES: {{^}}bool FalseBool = bool(FALSE_MACRO);{{$}}

void boolFunction(bool bar) {

}

char Character = 0; // OK

unsigned long long LongInteger = 1; // OK

#define MACRO_DEPENDENT_CAST(x) static_cast<bool>(x)
// CHECK-FIXES: {{^}}#define MACRO_DEPENDENT_CAST(x) static_cast<bool>(x){{$}}

bool MacroDependentBool = MACRO_DEPENDENT_CAST(0);
// CHECK-MESSAGES: :[[@LINE-1]]:27: warning: {{.*}}
// CHECK-FIXES: {{^}}bool MacroDependentBool = MACRO_DEPENDENT_CAST(0);{{$}}

bool ManyMacrosDependent = MACRO_DEPENDENT_CAST(FALSE_MACRO);
// CHECK-MESSAGES: :[[@LINE-1]]:49: warning: {{.*}}
// CHECK-FIXES: {{^}}bool ManyMacrosDependent = MACRO_DEPENDENT_CAST(FALSE_MACRO);{{$}}

class FooClass {
  public:
  FooClass() : JustBool(0) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: {{.*}}
  // CHECK-FIXES: {{^ *}}FooClass() : JustBool(false) {}{{$}}
  FooClass(int) : JustBool{0} {}
  // CHECK-MESSAGES: :[[@LINE-1]]:28: warning: {{.*}}
  // CHECK-FIXES: {{^ *}}FooClass(int) : JustBool{false} {}{{$}}
  private:
  bool JustBool;
  bool BoolWithBraces{0};
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: {{.*}}
  // CHECK-FIXES: {{^ *}}bool BoolWithBraces{false};{{$}}
  bool BoolFromInt = 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: {{.*}}
  // CHECK-FIXES: {{^ *}}bool BoolFromInt = false;{{$}}
  bool SimpleBool = true; // OK
};

template<typename type>
void templateFunction(type) {
  type TemplateType = 0;
  // CHECK-FIXES: {{^ *}}type TemplateType = 0;{{$}}
}

template<int c>
void valueDependentTemplateFunction() {
  bool Boolean = c;
  // CHECK-FIXES: {{^ *}}bool Boolean = c;{{$}}
}

template<typename type>
void anotherTemplateFunction(type) {
  bool JustBool = 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: {{.*}}
  // CHECK-FIXES: {{^ *}}bool JustBool = false;{{$}}
}

int main() {
  boolFunction(1);
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: {{.*}}
  // CHECK-FIXES: {{^ *}}boolFunction(true);{{$}}

  boolFunction(false);

  templateFunction(0);

  templateFunction(false);

  valueDependentTemplateFunction<1>();

  anotherTemplateFunction(1);

  IntToTrue = 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: {{.*}}
  // CHECK-FIXES: {{^ *}}IntToTrue = true;{{$}}
}
