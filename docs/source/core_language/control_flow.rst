Control flow and conditional parsing
====================================

The pattern language offers multiple ways to modify the behaviour of the parser on the go based on the values already parsed, allowing you to parse complex formats with relative ease.

Conditionals
^^^^^^^^^^^^

In the pattern language, not all structs need to be the same size or have the same layout. It's possible to
change what variables are getting declared based on the value of other variables.

.. code-block:: hexpat

    enum Type : u8 {
        A = 0x54,
        B = 0xA0,
        C = 0x0B
    };

    struct PacketA {
        float x;
    };

    struct PacketB {
        u8 y;
    };

    struct PacketC {
        char z[];
    };

    struct Packet {
        Type type;
        
        if (type == Type::A) {
            PacketA packet;
        }
        else if (type == Type::B) {
            PacketB packet;
        }
        else if (type == Type::C)
            PacketC packet;
    };

    Packet packet[3] @ 0xF0;

This code looks at the first byte of each ``Packet`` to determine its type so it can decode the body of the packet accordingly using the correct types defined in ``PacketA``, ``PacketB`` and ``PacketC``.

Conditionals like this can be used in Structs, Unions and Bitfields :version:`1.18.0`.

.. image:: assets/conditionals/hex.png
  :width: 100%
  :alt: Conditional Highlighing

.. image:: assets/conditionals/data.png
  :width: 100%
  :alt: Conditional Decoding


Pattern control flow :version:`1.13.0`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The most basic form of conditional parsing are array control flow statements, ``break`` and ``continue``. These allow you to stop the parsing of the array or skip elements based on conditions in the currently parsed item instance.

Break :version:`1.13.0`
-----------------------

When a break is reached, the current array creation process is terminated. 
This means, the array keeps all entries that have already been parsed, including the one that's being currently processed, but won't expand further, even if the requested number of entries hasn't been reached yet.

.. code-block:: hexpat

  struct Test {
    u32 x;

    if (x == 0x11223344)
      break;
  };

  // This array requests 1000 entries but stops growing as soon as it hits a u32 with the value 0x11223344
  // causing it to have a size less than 1000
  Test tests[1000] @ 0x00;


``break`` can also be used in regular patterns to prematurely stop parsing of the current pattern. :version:`1.24.0`
If the pattern where ``break`` is being used in is nested inside of another pattern, only evaluation of the current pattern is being stopped and continues in the parent struct after the definition of the current pattern.


Continue :version:`1.13.0`
--------------------------

When a continue is reached, the currently evaluated array entry gets evaluated to find next array entry offset but then gets discarded. 
This can be used to conditionally exclude certain array entries from the list that are either invalid or shouldn't be displayed in the pattern data list
while still scanning the entire range the array would span.

This can for instance be used in combination with :doc:`in/out variables </core_language/in_out>` to easily filter array items.

.. code-block:: hexpat

  struct Test {
    u32 value;

    if (value == 0x11223344)
      continue;
  };

  // This array requests 1000 entries but skips all entries where x has the value 0x11223344
  // causing it to have a size less than 1000
  Test tests[1000] @ 0x00;

``continue`` can also be used in regular patterns to discard the pattern entirely. :version:`1.24.0`
If the pattern where ``continue`` is being used in is nested inside of another pattern, only the current pattern is being discarded and evaluation continues in the parent struct after the definition of the current pattern.

Return statements :version:`1.24.0`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Return statements outside of functions can be used to prematurely terminate execution of the current program.

Evaluation stops at the location the ``return`` statement was executed. All patterns that have been evaluated up until this point will be finished up and placed into memory before the execution will halt.