cairo_font_extents_t fe;
cairo_text_extents_t te;
char alphabet[] = "AbCdEfGhIjKlMnOpQrStUvWxYz";
char letter[2];

cairo_font_extents (cr, &fe);
for (i=0; i < strlen(alphabet); i++) {
    *letter = '\0';
    strncat (letter, alphabet + i, 1);

    cairo_text_extents (cr, letter, &te);
    cairo_move_to (cr, i + 0.5 - te.x_bearing - te.width / 2,
            0.5 - fe.descent + fe.height / 2);
    cairo_show_text (cr, letter);
}
