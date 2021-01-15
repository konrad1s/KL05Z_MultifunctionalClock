#include "../inc/circular_buff.h"

// init circular buffor
uint8_t CB_init(circular_buff *cbuff)
{
    cbuff->data = (uint8_t *)malloc(CB_MAX_LEN);
    cbuff = malloc(sizeof(cbuff));
    return cbuff->buffor;
}

// delete buffor
void CB_free(circular_buff *cbuff)
{
    free(cbuff);
}

// checks if the buffer is full
CB_state CB_buff_full(circular_buff *cbuff)
{
    if (cbuff->new_len == CB_MAX_LEN)
        return buffor_full;

    else
        return buffor_not_full;
}

// checks if the buffer is empty
CB_state CB_buff_empty(circular_buff *cbuff)
{
    if (cbuff->new_len == 0)
        return buffor_empty;

    else
        return buffor_not_empty;
}

//return -1 if read data not success
uint8_t CB_read_data(circular_buff *cbuff)
{
    uint8_t r;
    // checks if the buffer is not empty
    if (CB_buff_empty(cbuff) == buffor_not_empty)
    {
        // if tail is at last position
        if (cbuff->tail == ((cbuff->buffor) + (CB_MAX_LEN)) - 1)
        {
            // put tail at the base address of the buffer
            cbuff->tail = cbuff->buffor;
            r = *(cbuff->tail);
            *(cbuff->tail) = 0;
            (cbuff->new_len)--;
            return r;
        }

        else
        {
            // remove data at the location of tail
            (cbuff->tail)++;
            r = *(cbuff->tail);
            *(cbuff->tail) = 0;
            (cbuff->new_len)--;
            return r;
        }
    }
    else
        // If buffer is empty return -1
        return -1;
}

//return 1 if add data success
uint8_t CB_add_data(circular_buff *cbuff, uint8_t data)
{
    // checks if buffer is not full
    if (CB_buff_full(cbuff) == buffor_not_full)
    {
        // if head is at last position
        if (cbuff->head == (cbuff->buffor) + (CB_MAX_LEN)-1)
        {
            //put head at the base address
            cbuff->head = cbuff->buffor;
            *(cbuff->head) = data;
            (cbuff->new_len)++;
            return 1;
        }

        else
        {
            // add data at the location of head
            cbuff->head++;
            *(cbuff->head) = data;
            (cbuff->new_len)++;
            return 1;
        }
    }
    else
        return 0;
}
