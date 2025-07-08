# Bug Tracking Log

This document tracks identified bugs, their status, and resolution.

| Bug ID | Date Reported | Reported By | Description | Priority | Status | Date Resolved | Resolution |
| :----- | :------------ | :---------- | :---------- | :------- | :----- | :------------ | :--------- |
| B001   | YYYY-MM-DD    | Gemini      | `main.cpp` uses unsafe `static_cast` to handle Admin login transition. Needs refactoring. | Medium   | Open   | YYYY-MM-DD    | [Details of the fix] |
| B002   | YYYY-MM-DD    | Gemini      | Customer Order History (`showCustomerDashboard`) shows all orders instead of filtering by user. Needs `Order::viewOrdersForUser`. | Low      | Open   | YYYY-MM-DD    | [Details of the fix] |
| B003   | YYYY-MM-DD    | Gemini      | Order placement in `showCustomerDashboard` requires `Order` object to be explicitly initialized with UserID before calling `placeOrder`. | Low      | Open   | YYYY-MM-DD    | [Details of the fix] |
|        |               |             |             |          |        |               |            |

## Status Definitions:

*   **Open**: Bug reported / known issue / required refinement.
*   **In Progress**: Bug is being actively worked on.
*   **Resolved**: A fix has been implemented and awaits testing/verification.
*   **Closed**: Bug fix verified, or bug deemed invalid/won't fix. 