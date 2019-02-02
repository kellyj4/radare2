/* radare - LGPL - Copyright 2018-2019 - pancake */

#include <r_asm.h>

R_API RAsmOp *r_asm_op_new() {
	return R_NEW0 (RAsmOp);
}

R_API void r_asm_op_free(RAsmOp *op) {
	r_asm_op_fini (op);
	free (op);
}

R_API void r_asm_op_init(RAsmOp *op) {
	memset (op, 0, sizeof (*op));
}

R_API void r_asm_op_fini(RAsmOp *op) {
	r_strbuf_fini (&op->buf);
	r_strbuf_fini (&op->buf_asm);
	r_strbuf_fini (&op->buf_hex);
	r_buf_fini (op->buf_inc);
}

// accessors
R_API char *r_asm_op_get_hex(RAsmOp *op) {
	return r_strbuf_get (&op->buf_hex);
}

R_API char *r_asm_op_get_asm(RAsmOp *op) {
	return r_strbuf_get (&op->buf_asm);
}

R_API ut8 *r_asm_op_get_buf(RAsmOp *op) {
	r_return_val_if_fail (op, NULL);
	return (ut8*)r_strbuf_get (&op->buf);
}

R_API int r_asm_op_get_size(RAsmOp *op) {
	r_return_val_if_fail (op, 1);
	const int len = op->size - op->payload;
	return R_MAX (1, len);
}

R_API void r_asm_op_set_asm(RAsmOp *op, const char *str) {
	r_return_if_fail (op && str);
	r_strbuf_set (&op->buf_asm, str);
}

R_API void r_asm_op_set_hex(RAsmOp *op, const char *str) {
	r_strbuf_set (&op->buf_hex, str);
	ut8 *bin = (ut8*)strdup (str);
	if (bin) {
		int len = r_hex_str2bin (str, bin);
		if (len > 0) {
			r_strbuf_setbin (&op->buf, bin, len);
		}
		free (bin);
	}
}

R_API void r_asm_op_set_hexbuf(RAsmOp *op, const ut8 *buf, int len) {
	r_return_if_fail (op && buf && len >= 0);
	char *hex = malloc (len * 4 + 1);
	if (hex) {
		r_hex_bin2str (buf, len, hex);
		r_asm_op_set_hex (op, hex);
		free (hex);
	}
	// TODO: update the op->buf too?
}

R_API void r_asm_op_set_buf(RAsmOp *op, const ut8 *buf, int len) {
	r_return_if_fail (op && buf && len >= 0);
	r_strbuf_setbin (&op->buf, buf, len);
	r_asm_op_set_hexbuf (op, buf, len);
}
