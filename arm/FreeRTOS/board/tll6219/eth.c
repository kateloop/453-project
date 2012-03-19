/****************************************************************************
 *
 * Copyright (c) 2012, The University of Wisconsin - Madison
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of The University of Wisconsin - Madison nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERISTY OF
 * WISCONSIN - MADISON BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Christopher Karle <ckarle@wisc.edu>
 *
 ****************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "lwip/netif.h"
#include "lwip/netifapi.h"
#include "lwip/pbuf.h"
#include "netif/etharp.h"
#include "FreeRTOS.h"
#include "iodefine.h"
#include "semphr.h"
#include "task.h"

/****************************************************************************
 *
 ****************************************************************************/
#define printf xprintf

/****************************************************************************
 *
 ****************************************************************************/
#define ETH_IFNAME0  's'
#define ETH_IFNAME1  'm'

/****************************************************************************
 *
 ****************************************************************************/
#define REG_BASE      ((volatile uint32_t*) 0xD1000000)
#define RX_DATA       (0x00 / 4)
#define TX_DATA       (0x20 / 4)
#define RX_STS        (0x40 / 4)
#define RX_PEEK       (0x44 / 4)
#define TX_STS        (0x48 / 4)
#define TX_PEEK       (0x4C / 4)
#define ID_REV        (0x50 / 4)
#define IRQ_CFG       (0x54 / 4)
#define INT_STS       (0x58 / 4)
#define INT_EN        (0x5C / 4)
#define BYTE_TEST     (0x64 / 4)
#define FIFO_INT      (0x68 / 4)
#define RX_CFG        (0x6C / 4)
#define TX_CFG        (0x70 / 4)
#define HW_CFG        (0x74 / 4)
#define RX_DP_CTRL    (0x78 / 4)
#define RX_FIFO_INF   (0x7C / 4)
#define TX_FIFO_INF   (0x80 / 4)
#define PMT_CTRL      (0x84 / 4)
#define GPIO_CFG      (0x88 / 4)
#define GTP_CFG       (0x8C / 4)
#define GTP_CNT       (0x90 / 4)
#define WORD_SWAP     (0x98 / 4)
#define FREE_RUN      (0x9C / 4)
#define RX_DROP       (0xA0 / 4)
#define MAC_CSR_CMD   (0xA4 / 4)
#define MAC_CSR_DATA  (0xA8 / 4)
#define AFC_CFG       (0xAC / 4)
#define E2P_CMD       (0xB0 / 4)
#define E2P_DATA      (0xB4 / 4)

/****************************************************************************
 *
 ****************************************************************************/
#define MAC_CR     1
#define ADDRH      2
#define ADDRL      3
#define HASHH      4
#define HASHL      5
#define MII_ACC    6
#define MII_DATA   7
#define FLOW       8
#define VLAN1      9
#define VLAN2     10
#define WUFF      11
#define WUCSR     12

/****************************************************************************
 *
 ****************************************************************************/
#define MII_BMCR       0
#define MII_BMSR       1
#define MII_PHYSID1    2
#define MII_PHYSID2    3
#define MII_ADVERTISE  4
#define MII_LPA        5
#define MII_EXPANSION  6

/****************************************************************************
 *
 ****************************************************************************/
#define PHY_ADDR  0x01

/****************************************************************************
 *
 ****************************************************************************/
static xSemaphoreHandle rxLock = NULL;
static xSemaphoreHandle txLock = NULL;
static struct netif netif;

#if configUSE_DHCP == 1
/****************************************************************************
 *
 ****************************************************************************/
static void dhcpTask(void* ptr)
{
   int state = 0;

   for (;;)
   {
      vTaskDelay(250 * portTICK_RATE_MS);

      if (state == 0)
      {
         if (netif_is_link_up(&netif))
         {
            netifapi_dhcp_start(&netif);
            state = 1;
         }
         else
         {
            continue;
         }
      }

      if (!netif_is_link_up(&netif))
      {
         struct ip_addr gateway = { .addr = 0x00000000 };
         struct ip_addr netmask = { .addr = 0x00000000 };
         struct ip_addr ip = { .addr = 0x00000000 };

         netifapi_dhcp_stop(&netif);
         netif_set_down(&netif);
         netif_set_addr(&netif, &ip, &netmask, &gateway);

         state = 0;
      }

      if (state == 1)
      {
         if (netif_is_up(&netif) && netif.ip_addr.addr)
         {
            printf("Bound to IP %d.%d.%d.%d\n",
                   (0x000000FF & (netif.ip_addr.addr >>  0)),
                   (0x000000FF & (netif.ip_addr.addr >>  8)),
                   (0x000000FF & (netif.ip_addr.addr >> 16)),
                   (0x000000FF & (netif.ip_addr.addr >> 24)));

            state = 2;
         }
      }
   }
}
#endif

/****************************************************************************
 *
 ****************************************************************************/
static uint32_t csrRead(uint8_t address)
{
   REG_BASE[MAC_CSR_CMD] = 0xC0000000 | address;

   while (REG_BASE[MAC_CSR_CMD] & 0x80000000);

   return REG_BASE[MAC_CSR_DATA];
}

/****************************************************************************
 *
 ****************************************************************************/
static void csrWrite(uint8_t address, uint32_t value)
{
   REG_BASE[MAC_CSR_DATA] = value;

   REG_BASE[MAC_CSR_CMD] = 0x80000000 | address;

   while (REG_BASE[MAC_CSR_CMD] & 0x80000000);
}

/****************************************************************************
 *
 ****************************************************************************/
static uint16_t miiRead(uint8_t phy, uint8_t address)
{
   csrWrite(MII_ACC, (phy << 11) | (address << 6) | 0x00000001);

   while (csrRead(MII_ACC) & 0x00000001);

   return (uint16_t) csrRead(MII_DATA);
}

/****************************************************************************
 *
 ****************************************************************************/
static void miiWrite(uint8_t phy, uint8_t address, uint16_t value)
{
   csrWrite(MII_DATA, value);

   csrWrite(MII_ACC, (phy << 11) | (address << 6) | 0x00000003);

   while (csrRead(MII_ACC) & 0x00000001);
}

/****************************************************************************
 *
 ****************************************************************************/
static void ethISR()
{
   uint32_t status = REG_BASE[INT_STS];

   if (status & 0x00040000)
   {
      uint16_t bmsr = miiRead(PHY_ADDR, MII_BMSR);
      const char* speed = "unknown speed";

      /* You should always avoid printing from interrupt service routines,
       * but link state changes should be very rare.
       */
      if (bmsr & 0x0004)
      {
         if (bmsr & 0x4000)
            speed = "100Mbps";
         else if (bmsr & 0x2000)
            speed = "100Mbps half-duplex";
         else if (bmsr & 0x1000)
            speed = "10Mbps";
         else if (bmsr & 0x0800)
            speed = "10Mbps half-duplex";

         netif.flags |= NETIF_FLAG_LINK_UP | NETIF_FLAG_UP;
      }
      else
      {
         speed = "down";
         netif.flags &= ~(NETIF_FLAG_LINK_UP | NETIF_FLAG_UP);
      }

      printf("eth link: %s\n", speed);

      miiRead(PHY_ADDR, 29);
   }

   if ((REG_BASE[RX_FIFO_INF] >> 16) & 0x000000FF)
   {
      portBASE_TYPE yield = pdFALSE;

      xSemaphoreGiveFromISR(rxLock, &yield);

      if (yield == pdTRUE)
         vTaskSwitchContext();

      REG_BASE[INT_EN] &= ~0x00000008;
   }

   REG_BASE[INT_STS] = status;
   PTF_ISR = 0x00000001;
}

/****************************************************************************
 *
 ****************************************************************************/
static err_t ethTx(struct netif* netif, struct pbuf* pbuf)
{
   struct pbuf* q = NULL;

   while (xSemaphoreTake(txLock, portMAX_DELAY) == pdFALSE);

   while ((REG_BASE[TX_FIFO_INF] >> 16) & 0x000000FF)
      REG_BASE[TX_STS];

   for (q = pbuf; q != NULL; q = q->next)
   {
      uint32_t size = q->len;
      uint32_t* ptr = NULL;
      uint32_t cmd;
      uint32_t i;

      if (size % 4)
         size += 4 - (size % 4);

      while ((REG_BASE[TX_FIFO_INF] & 0x0000FFFF) < (size + 8))
         taskYIELD();

      ptr = (uint32_t*) ((uintptr_t) q->payload & ~0x00000003);

      cmd = (((uintptr_t) q->payload & 0x00000003) << 16) | q->len;

      if (q == pbuf)
         cmd |= 0x00002000;
      if (q->next == NULL)
         cmd |= 0x00001000;

      REG_BASE[TX_DATA] = cmd;
      REG_BASE[TX_DATA] = q->tot_len;

      for (i = 0; i < (size / 4); i++)
         REG_BASE[TX_DATA] = ptr[i];
   }

   xSemaphoreGive(txLock);

   return ERR_OK;
}

/****************************************************************************
 *
 ****************************************************************************/
static void ethTask(void* ptr)
{
   for (;;)
   {
      while (xSemaphoreTake(rxLock, portMAX_DELAY) == pdFALSE);

      while (((REG_BASE[RX_FIFO_INF] >> 16) & 0x000000FF) > 0)
      {
         uint32_t status = REG_BASE[RX_STS];
         uint32_t size = (status >> 16) & 0x3FFF;
         struct pbuf* pbuf = NULL;
         uint32_t i;

         if (status & 0x00008000)
         {
            if (size < 16)
            {
               REG_BASE[RX_DATA];
               REG_BASE[RX_DATA];
               REG_BASE[RX_DATA];
               REG_BASE[RX_DATA];
            }
            else
            {
               REG_BASE[RX_DP_CTRL] = 0x80000000;
               while (REG_BASE[RX_DP_CTRL] & 0x80000000)
                   taskYIELD();
            }

            continue;
         }

         if (size % 4)
            size += 4 - (size % 4);

         pbuf = pbuf_alloc(PBUF_RAW, size, PBUF_RAM);

         if ((uintptr_t) pbuf->payload & 0x00000003)
            printf("warning: unaligned pbuf->payload\n");

         for (i = 0; i < (pbuf->len / 4); i++)
            ((uint32_t*) pbuf->payload)[i] = REG_BASE[RX_DATA];

         if (netif.input(pbuf, &netif) != ERR_OK)
            pbuf_free(pbuf);
      }

      REG_BASE[INT_STS] = 0x00000008;
      REG_BASE[INT_EN] |= 0x00000008;
   }
}

/****************************************************************************
 *
 ****************************************************************************/
static err_t _ethInit()
{
   uint32_t addrh = csrRead(ADDRH);
   uint32_t addrl = csrRead(ADDRL);

   /* reset the MAC */
   REG_BASE[HW_CFG] = 0x00000001;
   while (REG_BASE[HW_CFG] & 0x00000001);

   /* enable MBO bit */
   REG_BASE[HW_CFG] |= 0x00100000;

   /* wait for the EEPROM controller to finish */
   while (REG_BASE[E2P_CMD] & 0x80000000);

   /* flow control */
   REG_BASE[AFC_CFG] = 0x006E3740;

   /* restore the MAC address */
   csrWrite(ADDRH, addrh);
   csrWrite(ADDRL, addrl);

   /* LED pins */
   REG_BASE[GPIO_CFG] = 0x70070000;

   /* reset the PHY */
   miiWrite(PHY_ADDR, MII_BMCR, 0x8000);
   while (miiRead(PHY_ADDR, MII_BMCR) & 0x8000);
   mDelay(1);

   /* set auto-negotiation advertisement */
   miiWrite(PHY_ADDR, MII_ADVERTISE, 0x01E1);

   /* set interrupt sources for PHY */
   miiWrite(PHY_ADDR, 30, 0x0050);

   /* set interrupt sources for MAC */
   REG_BASE[INT_EN] |= 0x00040008;

   /* turn on master IRQ enable */
   REG_BASE[IRQ_CFG] = 0x0A000100;
 
   /* enable transmitter */
   REG_BASE[TX_CFG] |= 0x00000002;

   /* enable the rx/tx at the MAC */
   csrWrite(MAC_CR, csrRead(MAC_CR) | 0x0000000C);

   /* restart auto-negotiation */
   miiWrite(PHY_ADDR, MII_BMCR, 0x1200);

   return ERR_OK;
}

/****************************************************************************
 *
 ****************************************************************************/
bool ethIsUp()
{
   if (netif_is_link_up(&netif) && netif_is_up(&netif) && netif.ip_addr.addr)
      return true;

   return false;
}

/****************************************************************************
 *
 ****************************************************************************/
void ethInit()
{
   uint32_t addrh = csrRead(ADDRH);
   uint32_t addrl = csrRead(ADDRL);
   struct ip_addr gateway;
   struct ip_addr netmask;
   struct ip_addr ip;

#if configUSE_DHCP == 1
   gateway.addr = 0x00000000;
   netmask.addr = 0x00000000;
   ip.addr = 0x00000000;
#else
   IP4_ADDR(&gateway, configGATEWAY_3, configGATEWAY_2, configGATEWAY_1,
            configGATEWAY_0);
   IP4_ADDR(&netmask, configNETMASK_3, configNETMASK_2, configNETMASK_1,
            configNETMASK_0);
   IP4_ADDR(&ip, configIP_3, configIP_2, configIP_1, configIP_0);
#endif

   /* configure the interrupt pin */
   PTF_GIUS |= 0x00000001;
   PTF_DDIR &= 0xFFFFFFFE;
   PTF_ICR1 |= 0x00000003;
   PTF_PUEN |= 0x00000001;
   PTF_IMR |= 0x00000001;
   PMASK |= 0x00000020;

   netif.state = NULL;
   netif.mtu = 1500;
   netif.name[0] = ETH_IFNAME0;
   netif.name[1] = ETH_IFNAME1;
   netif.num = 0;

   netif.hwaddr_len = ETHARP_HWADDR_LEN;
   netif.hwaddr[0] = 0xFF & (addrl >>  0);
   netif.hwaddr[1] = 0xFF & (addrl >>  8);
   netif.hwaddr[2] = 0xFF & (addrl >> 16);
   netif.hwaddr[3] = 0xFF & (addrl >> 24);
   netif.hwaddr[4] = 0xFF & (addrh >>  0);
   netif.hwaddr[5] = 0xFF & (addrh >>  8);

   netif.output = etharp_output;
   netif.linkoutput = ethTx;

   netif_add(&netif, &ip, &netmask, &gateway, NULL, _ethInit, ethernet_input);
   netif_set_default(&netif);
   netif.flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

   vSemaphoreCreateBinary(rxLock);
   txLock = xSemaphoreCreateMutex();

   /* start the task that acts as the bottom half of the processing */
   xTaskCreate(ethTask, (signed char*) "smsc9115", configMINIMAL_STACK_SIZE,
               NULL, configMAX_PRIORITIES - 1, NULL);

   /* allow interrupts at the CPU */
   configIRQ_HANDLER(8) = ethISR;
   NIPRIORITY1 &= 0xFFFFFFF0;
   NIPRIORITY1 |= configETH_INTERRUPT_PRIORITY & 0xF;
   INTENNUM = 8;

#if configUSE_DHCP == 1
   /* start the DHCP task */
   xTaskCreate(dhcpTask, (signed char*) "dhcp", configMINIMAL_STACK_SIZE,
               NULL, configMAX_PRIORITIES - 1, NULL);
#endif
}
